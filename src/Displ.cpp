#include "Displ.h"

#include "macros.h"

Displ::Displ(const char *name, const State &state, const std::list<ModeP> &modes, const ModeP &currentMode,
             const ModeP preferredMode, const ModeP optimalMode, const PosP currentPos) :
        name(name), state(state), modes(modes), currentMode(currentMode), preferredMode(preferredMode),
        optimalMode(optimalMode), currentPos(currentPos) {
    if (name == NULL) FAIL("Displ has no name")
    switch (state) {
        case active:
            if (!currentMode) FAIL("active Displ %s has no currentMode", name)
            if (!currentPos) FAIL("active Displ %s has no currentPos", name)
            if (!optimalMode) FAIL("active Displ %s has no optimalMode", name)
            if (modes.empty()) FAIL("active Displ %s has no modes", name)
            break;
        case connected:
            if (!optimalMode) FAIL("connected Displ %s has no optimalMode", name)
            if (modes.empty()) FAIL("connected Displ %s has no modes", name)
            break;
        default:
            break;
    }
}
