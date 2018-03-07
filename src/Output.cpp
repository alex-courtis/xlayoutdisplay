#include "Output.h"
#include "calculations.h"

#include <algorithm>

using namespace std;

Output::Output(const string &name,
             const State &state,
             const list<shared_ptr<Mode>> &modes,
             const shared_ptr<Mode> &currentMode,
             const shared_ptr<Mode> &preferredMode,
             const shared_ptr<Pos> &currentPos,
             const shared_ptr<Edid> &edid) :
        name(name),
        state(state),
        modes(modes),
        currentMode(currentMode),
        preferredMode(preferredMode),
        optimalMode(calculateOptimalMode(modes, preferredMode)),
        currentPos(currentPos),
        edid(edid) {
    switch (state) {
        case active:
            if (!currentMode) throw invalid_argument("active Output '" + name + "' has no currentMode");
            if (!currentPos) throw invalid_argument("active Output '" + name + "' has no currentPos");
            if (modes.empty()) throw invalid_argument("active Output '" + name + "' has no modes");
            break;
        case connected:
            if (modes.empty()) throw invalid_argument("connected Output '" + name + "' has no modes");
            break;
        default:
            break;
    }

    // active / connected must have NULL or valid preferred mode
    if (state == active || state == connected) {
        if (preferredMode && find(this->modes.begin(), this->modes.end(), preferredMode) == this->modes.end())
            throw invalid_argument("Output '" + name + "' has preferredMode not present in modes");
    }
}

bool Output::desiredActive() const {
    return _desiredActive;
}

void Output::desiredActive(const bool desiredActive) {
    if (desiredActive && !optimalMode)
        throw invalid_argument("Output '" + name + "' cannot set desiredActive without optimalMode");
    _desiredActive = desiredActive;
}

const shared_ptr<Mode> &Output::desiredMode() const {
    return _desiredMode;
}

void Output::desiredMode(const shared_ptr<Mode> &desiredMode) {
    if (find(modes.begin(), modes.end(), desiredMode) == this->modes.end())
        throw invalid_argument("Output '" + name + "' cannot set desiredMode which is not present in modes");
    this->_desiredMode = desiredMode;
}
