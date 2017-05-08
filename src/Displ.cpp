#include "Displ.h"
#include "util.h"

#include <algorithm>

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
