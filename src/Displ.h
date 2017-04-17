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
    //   active must have: currentMode, currentPos, optimalMode, modes
    //   connected must have: optimalMode, modes
    Displ(const std::string &name, const State &state, const std::list<ModeP> &modes, const ModeP &currentMode, const ModeP &preferredMode,
          const ModeP &optimalMode, const PosP &currentPos, const EdidP edid);

    virtual ~Displ() {
    }

    const std::string name;
    const State state;

    const std::list<ModeP> modes;   // should be ordered descending when constructing
    const ModeP currentMode;
    const ModeP preferredMode;
    const ModeP optimalMode; // todo: use preferredMode res with highest refresh, but must consider interlacing, 3d etc.
    ModeP desiredMode;

    const PosP currentPos;
    PosP desiredPos;

    const EdidP edid;

    bool desiredActive = false;

    static std::shared_ptr<Displ> desiredPrimary;
};

typedef std::shared_ptr<Displ> DisplP;

#endif //XLAYOUTDISPLAYS_DISPL_H
