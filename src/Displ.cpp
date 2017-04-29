#include "Displ.h"
#include "util.h"

#include <algorithm>

using namespace std;

DisplP Displ::desiredPrimary;

Displ::Displ(const string &name, const State &state, const std::list<ModeP> &modes, const ModeP &currentMode, const ModeP &preferredMode,
             const PosP &currentPos, const EdidP edid) :
        name(name), state(state), modes(reverseSharedPtrList(sortSharedPtrList(modes))), currentMode(currentMode), preferredMode(preferredMode),
        currentPos(currentPos), edid(edid) {

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

    // default optimal mode is empty
    if (!this->modes.empty()) {

        // use highest mode for optimal
        optimalMode = this->modes.front();

        // override with highest refresh of preferred
        if (this->preferredMode)
            for (const ModeP mode : this->modes)
                if (mode->width == preferredMode->width && mode->height == preferredMode->height) {
                    optimalMode = mode;
                    break;
                }
    }

    // currentMode must be in modes
    if (currentMode && find(this->modes.begin(), this->modes.end(), currentMode) == this->modes.end())
        throw invalid_argument("Displ '" + name + "' has currentMode not present in modes");

    // preferredMode must be in modes
    if (preferredMode && find(this->modes.begin(), this->modes.end(), preferredMode) == this->modes.end())
        throw invalid_argument("Displ '" + name + "' has preferredMode not present in modes");
}

bool Displ::isDesiredActive() const {
    return desiredActive;
}

void Displ::setDesiredActive() {
    if (!optimalMode)
        throw invalid_argument("cannot set desiredActive for a Displ without optimalMode");
    desiredActive = true;
}

const ModeP &Displ::getOptimalMode() const {
    return optimalMode;
}
