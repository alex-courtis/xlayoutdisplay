#include "Displ.h"

using namespace std;

Displ::Displ(const string &name, const State &state, const std::list<ModeP> &modes, const ModeP &currentMode,
             const ModeP &preferredMode, const ModeP &optimalMode, const PosP &currentPos) :
        name(name), state(state), modes(modes), currentMode(currentMode), preferredMode(preferredMode),
        optimalMode(optimalMode), currentPos(currentPos) {

    switch (state) {
        case active:
            if (!currentMode) throw invalid_argument("active Displ '" + name + "' has no currentMode");
            if (!currentPos) throw invalid_argument("active Displ '" + name + "' has no currentPos");
            if (!optimalMode) throw invalid_argument("active Displ '" + name + "' has no optimalMode");
            if (modes.empty()) throw invalid_argument("active Displ '" + name + "' has no modes");
            break;
        case connected:
            if (!optimalMode) throw invalid_argument("connected Displ '" + name + "' has no optimalMode");
            if (modes.empty()) throw invalid_argument("connected Displ '" + name + "' has no modes");
            break;
        default:
            break;
    }
}
