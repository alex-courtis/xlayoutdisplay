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
    // todo: add preconditions e.g. in mode
    ModeP desiredMode;

    const PosP currentPos;
    PosP desiredPos;

    const EdidP edid;

    // must have been set during construction
    const ModeP &getOptimalMode() const;

    // optimalMode must be present
    bool isDesiredActive() const;
    void setDesiredActive();

    static std::shared_ptr<Displ> desiredPrimary;

private:
    bool desiredActive = false;
    ModeP optimalMode;
};

typedef std::shared_ptr<Displ> DisplP;

#endif //XLAYOUTDISPLAYS_DISPL_H
