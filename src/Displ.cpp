#include "Displ.h"
#include "util.h"
#include "calculations.h"

#include <algorithm>

using namespace std;

// one and only primary
shared_ptr<Displ> Displ::desiredPrimary;

long Displ::desiredDpi = DEFAULT_DPI;

Displ::Displ(const string &name,
             const State &state,
             const list<shared_ptr<Mode>> &modes,
             const shared_ptr<Mode> &currentMode,
             const shared_ptr<Mode> &preferredMode,
             const shared_ptr<Pos> &currentPos,
             const shared_ptr<Edid> &edid) :
        name(name),
        state(state),
        modes(reverseSharedPtrList(sortSharedPtrList(modes))),
        currentMode(currentMode),
        preferredMode(preferredMode),
        optimalMode(generateOptimalMode(this->modes, preferredMode)),
        currentPos(currentPos),
        edid(edid) {
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

const shared_ptr<Mode> &Displ::desiredMode() const {
    return _desiredMode;
}

void Displ::desiredMode(const shared_ptr<Mode> &desiredMode) {
    if (find(modes.begin(), modes.end(), desiredMode) == this->modes.end())
        throw invalid_argument("Displ '" + name + "' cannot set desiredMode which is not present in modes");
    this->_desiredMode = desiredMode;
}
