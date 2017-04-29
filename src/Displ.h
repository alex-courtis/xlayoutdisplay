#ifndef XLAYOUTDISPLAYS_DISPL_H
#define XLAYOUTDISPLAYS_DISPL_H

#include "Mode.h"
#include "Pos.h"
#include "Edid.h"

#include <memory>
#include <list>

// a single Xrandr display
class Displ {
public:
    enum State {
        active, connected, disconnected
    };

    // throws invalid_argument:
    //   active must have: currentMode, currentPos, modes
    //   connected must have: modes
    // modes will be ordered descending
    // optimalMode will be set to highest refresh preferredMode, then highest mode, then empty
    Displ(const std::string &name, const State &state, const std::list<ModeP> &modes, const ModeP &currentMode, const ModeP &preferredMode,
          const PosP &currentPos, const EdidP edid);

    virtual ~Displ() {
    }

    const std::string name;
    const State state;

    const std::list<ModeP> modes;
    const ModeP currentMode;
    const ModeP preferredMode;
    // todo: make this const
    ModeP optimalMode;
    // todo: add preconditions e.g. in mode
    ModeP desiredMode;

    const PosP currentPos;
    PosP desiredPos;

    const EdidP edid;

    // todo: add preconditions i.e. optimalMode not null or a method that takes desiredMode, desiredPos
    bool desiredActive = false;

    static std::shared_ptr<Displ> desiredPrimary;
};

typedef std::shared_ptr<Displ> DisplP;

#endif //XLAYOUTDISPLAYS_DISPL_H
