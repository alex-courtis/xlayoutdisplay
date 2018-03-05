#include "calculations.h"
#include "util.h"

#include <sstream>
#include <cstring>

using namespace std;

void orderDispls(list<shared_ptr<Displ>> &displs, const list<string> &order) {

    // stack all the preferred, available displays
    list<shared_ptr<Displ>> preferredDispls;
    for (const auto &name : order) {
        for (const auto &displ : displs) {
            if (strcasecmp(name.c_str(), displ->name.c_str()) == 0) {
                preferredDispls.push_front(displ);
            }
        }
    }

    // move preferred to the front
    for (const auto &preferredDispl : preferredDispls) {
        displs.remove(preferredDispl);
        displs.push_front(preferredDispl);
    }
}

const shared_ptr<Displ> activateDispls(const list<shared_ptr<Displ>> &displs, const string &desiredPrimary, const Monitors &monitors) {
    if (displs.empty()) throw invalid_argument("activateDispls received empty displs");

    shared_ptr<Displ> primary;

    for (const auto &displ : displs) {

        // don't display any monitors that shouldn't
        if (monitors.shouldDisableDisplay(displ->name))
            continue;

        // only activate currently active or connected displays
        if (displ->state != Displ::active && displ->state != Displ::connected)
            continue;

        // mark active
        displ->desiredActive(true);

        // default first to primary
        if (!primary)
            primary = displ;

        // user selected primary
        if (!desiredPrimary.empty() && strcasecmp(desiredPrimary.c_str(), displ->name.c_str()) == 0)
            primary = displ;
    }

    if (!primary) throw runtime_error("no active or connected displays found");

    return primary;
}

void ltrDispls(list<shared_ptr<Displ>> &displs) {
    int xpos = 0;
    int ypos = 0;
    for (const auto &displ : displs) {

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

void mirrorDispls(list<shared_ptr<Displ>> &displs) {

    // find the first active display
    shared_ptr<Displ> firstDispl;
    for (const auto &displ : displs) {
        if (displ->desiredActive()) {
            firstDispl = displ;
            break;
        }
    }
    if (!firstDispl)
        return;

    // iterate through first active display's modes
    for (const auto &possibleMode : reverseSort(firstDispl->modes)) {
        bool matched = true;

        // attempt to match mode to each active displ
        for (const auto &displ : displs) {
            if (!displ->desiredActive())
                continue;

            // reset failed matches
            shared_ptr<Mode> desiredMode;

            // match height and width
            for (const auto &mode : reverseSort(displ->modes)) {
                if (mode->width == possibleMode->width && mode->height == possibleMode->height) {

                    // select best refresh
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

const string renderUserInfo(const list<shared_ptr<Displ>> &displs) {
    stringstream ss;
    for (const auto &displ : displs) {
        ss << displ->name;
        switch (displ->state) {
            case Displ::active:
                ss << " active";
                break;
            case Displ::connected:
                ss << " connected";
                break;
            case Displ::disconnected:
                ss << " disconnected";
                break;
        }
        if (displ->edid) {
            ss << ' ' << displ->edid->maxCmHoriz() << "cm/" << displ->edid->maxCmVert() << "cm";
        }
        if (displ->currentMode && displ->currentPos) {
            ss << ' ' << displ->currentMode->width << 'x' << displ->currentMode->height;
            ss << '+' << displ->currentPos->x << '+' << displ->currentPos->y;
            ss << ' ' << displ->currentMode->refresh << "Hz";
        }
        ss << endl;
        for (const auto &mode : displ->modes) {
            ss << (mode == displ->currentMode ? '*' : ' ');
            ss << (mode == displ->preferredMode ? '+' : ' ');
            ss << (mode == displ->optimalMode ? '!' : ' ');
            ss << mode->width << 'x' << mode->height << ' ' << mode->refresh << "Hz";
            ss << endl;
        }
    }
    ss << "*current +preferred !optimal";
    return ss.str();
}

const long calculateDpi(const shared_ptr<Displ> &displ, string &explaination) {
    if (!displ) throw invalid_argument("calculateDpi received empty displ");

    long dpi = DEFAULT_DPI;
    stringstream verbose;
    if (!displ->edid) {
        verbose << "DPI defaulting to "
                << dpi
                << "; EDID information not available for display "
                << displ->name;
    } else if (!displ->desiredMode()) {
        // TODO is this a realistic case? no, we need an "ActiveDisplay" subclass of Display
        verbose << "DPI defaulting to "
                << dpi
                << "; desiredMode not available for display "
                << displ->name;
    } else {
        const long caldulatedDpi = displ->edid->dpiForMode(displ->desiredMode());
        if (caldulatedDpi == 0) {
            verbose << "DPI defaulting to "
                    << dpi
                    << "; no display size EDID information available for "
                    << displ->name;
        } else {
            dpi = caldulatedDpi;
            verbose << "calculated DPI "
                    << dpi
                    << " for display "
                    << displ->name;
        }
    }

    explaination = verbose.str();
    return dpi;
}

const shared_ptr<Mode> calculateOptimalMode(const list<shared_ptr<Mode>> &modes, const shared_ptr<Mode> &preferredMode) {
    shared_ptr<Mode> optimalMode;

    // default optimal mode is empty
    if (!modes.empty()) {

        // use highest resolution/refresh for optimal
        const list<shared_ptr<Mode>> reverseOrderedModes = reverseSort(modes);
        optimalMode = reverseOrderedModes.front();

        // override with highest refresh of preferred resolution, if available
        if (preferredMode)
            for (auto &mode : reverseOrderedModes)
                if (mode->width == preferredMode->width && mode->height == preferredMode->height) {
                    optimalMode = mode;
                    break;
                }
    }

    return optimalMode;
}
