#include "Displ.h"
#include "Laptop.h"
#include "util.h"

#include <algorithm>
#include <sstream>
#include <string.h>

using namespace std;

// generate an optimal mode from a sorted list of modes and preferredMode
ModeP generateOptimalMode(const list <ModeP> &modes, const ModeP &preferredMode);

// one and only primary
DisplP Displ::desiredPrimary;

unsigned int Displ::desiredDpi = DEFAULT_DPI;

Displ::Displ(const string &name, const State &state, const list <ModeP> &modes, const ModeP &currentMode, const ModeP &preferredMode, const PosP &currentPos,
             const EdidP edid) :
        name(name), state(state), modes(reverseSharedPtrList(sortSharedPtrList(modes))), currentMode(currentMode), preferredMode(preferredMode),
        optimalMode(generateOptimalMode(this->modes, preferredMode)), currentPos(currentPos), edid(edid) {

    switch (state) {
        case active:
            if (!currentMode) throw invalid_argument("active Displ '" + name + "' has no currentMode");
            if (!currentPos) throw invalid_argument("active Displ '" + name + "' has no currentPos");
            if (modes.empty()) throw invalid_argument("active Displ '" + name + "' has no modes");
            break;
        case connected:
            if (modes.empty()) throw invalid_argument("connected Displ '" + name + "' has no modes");
            break;
        default:
            break;
    }

    // active / connected must have NULL or valid modes
    if (state == active || state == connected) {

        // currentMode must be in modes
        if (currentMode && find(this->modes.begin(), this->modes.end(), currentMode) == this->modes.end())
            throw invalid_argument("Displ '" + name + "' has currentMode not present in modes");

        // preferredMode must be in modes
        if (preferredMode && find(this->modes.begin(), this->modes.end(), preferredMode) == this->modes.end())
            throw invalid_argument("Displ '" + name + "' has preferredMode not present in modes");
    }
}

bool Displ::desiredActive() const {
    return _desiredActive;
}

void Displ::desiredActive(const bool desiredActive) {
    if (desiredActive && !optimalMode)
        throw invalid_argument("Displ '" + name + "' cannot set desiredActive without optimalMode");
    _desiredActive = desiredActive;
}

const ModeP &Displ::desiredMode() const {
    return _desiredMode;
}

void Displ::desiredMode(const ModeP &desiredMode) {
    if (find(modes.begin(), modes.end(), desiredMode) == this->modes.end())
        throw invalid_argument("Displ '" + name + "' cannot set desiredMode which is not present in modes");
    this->_desiredMode = desiredMode;
}

ModeP generateOptimalMode(const list <ModeP> &modes, const ModeP &preferredMode) {
    ModeP optimalMode;

    // default optimal mode is empty
    if (!modes.empty()) {

        // use highest mode for optimal
        optimalMode = modes.front();

        // override with highest refresh of preferred
        if (preferredMode)
            for (const ModeP mode : modes)
                if (mode->width == preferredMode->width && mode->height == preferredMode->height) {
                    optimalMode = mode;
                    break;
                }
    }

    return optimalMode;
}

void orderDispls(list <DisplP> &displs, const list <string> &order) {

    // stack all the preferred, available displays
    list <DisplP> preferredDispls;
    for (const auto name : order) {
        for (const auto displ : displs) {
            if (strcasecmp(name.c_str(), displ->name.c_str()) == 0) {
                preferredDispls.push_front(displ);
            }
        }
    }

    // move preferred to the front
    for (const auto preferredDispl : preferredDispls) {
        displs.remove(preferredDispl);
        displs.push_front(preferredDispl);
    }
}

void activateDispls(std::list<DisplP> &displs, const string &primary) {
    Laptop *laptop = Laptop::instance();
    for (const auto displ : displs) {

        // don't use any laptop displays if the lid is closed
        if (laptop->shouldDisableDisplay(displ->name))
            continue;

        // only activate currently active or connected displays
        if (displ->state != Displ::active && displ->state != Displ::connected)
            continue;

        // mark active
        displ->desiredActive(true);

        // default first to primary
        if (!Displ::desiredPrimary)
            Displ::desiredPrimary = displ;

        // user selected primary
        if (!primary.empty() && strcasecmp(primary.c_str(), displ->name.c_str()) == 0)
            Displ::desiredPrimary = displ;
    }
}

void ltrDispls(list <DisplP> &displs) {
    int xpos = 0;
    int ypos = 0;
    for (const auto displ : displs) {

        if (displ->desiredActive()) {

            // set the desired mode to optimal
            displ->desiredMode(displ->optimalMode);

            // position the screen
            displ->desiredPos = make_shared<Pos>(xpos, ypos);

            // next position
            xpos += displ->desiredMode()->width;
        }
    }
}

void mirrorDispls(list <DisplP> &displs) {

    // find the first active display
    DisplP firstDispl;
    for (const auto displ : displs) {
        if (displ->desiredActive()) {
            firstDispl = displ;
            break;
        }
    }
    if (!firstDispl)
        return;

    // iterate through first active display's modes
    for (const auto possibleMode : firstDispl->modes) {
        bool matched = true;

        // attempt to match mode to each active displ
        for (const auto displ : displs) {
            if (!displ->desiredActive())
                continue;

            // reset failed matches
            ModeP desiredMode;

            // match height and width only
            for (const auto mode : displ->modes) {
                if (mode->width == possibleMode->width && mode->height == possibleMode->height) {

                    // set mode and pos
                    desiredMode = mode;
                    break;
                }
            }

            // match a mode for every display; root it at 0, 0
            matched = matched && desiredMode;
            if (matched) {
                displ->desiredMode(desiredMode);
                displ->desiredPos = make_shared<Pos>(0, 0);
                continue;
            }
        }

        // we've set desiredMode and desiredPos (zero) for all displays, all done
        if (matched)
            return;
    }

    // couldn't find a common mode, exit
    throw runtime_error("unable to find common width/height for mirror");
}

// todo: document and test this; refactor needed
string calculateDpi(std::list<DisplP> &displs) {
    stringstream verbose;
    if (!Displ::desiredPrimary) {
        verbose << "DPI defaulting to " << Displ::desiredDpi << "; no primary display has been set set";
    } else if (!Displ::desiredPrimary->edid) {
        verbose << "DPI defaulting to " << Displ::desiredDpi << "; EDID information not available for primary display " << Displ::desiredPrimary->name;
    } else if (!Displ::desiredPrimary->desiredMode()) {
        verbose << "DPI defaulting to " << Displ::desiredDpi << "; desiredMode not available for primary display " << Displ::desiredPrimary->name;
    } else {
        const unsigned int desiredDpi = Displ::desiredPrimary->edid->closestDpiForMode(Displ::desiredPrimary->desiredMode());
        if (desiredDpi == 0) {
            verbose << "DPI defaulting to " << Displ::desiredDpi << "; no display size EDID information available for " << Displ::desiredPrimary->name;
        } else {
            Displ::desiredDpi = desiredDpi;
            verbose << "DPI rounded to " << Displ::desiredDpi << " for primary display " << Displ::desiredPrimary->name << " with DPI: "
                    << Displ::desiredPrimary->edid->dpiForMode(Displ::desiredPrimary->desiredMode());
        }
    }

    return verbose.str();
}
