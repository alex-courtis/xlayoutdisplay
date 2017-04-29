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
    const PosP currentPos;
    const EdidP edid;

    // todo: secure this as per desiredMode
    PosP desiredPos;

    // may be empty
    const ModeP &getOptimalMode() const;

    // may be empty
    const ModeP &getDesiredMode() const;

    // desiredMode must be in modes
    void setDesiredMode(const ModeP &desiredMode);

    // true ensures optimalMode available
    bool isDesiredActive() const;

    // optimalMode must be present
    void setDesiredActive();

    static std::shared_ptr<Displ> desiredPrimary;

private:
    ModeP optimalMode;
    ModeP desiredMode;
    bool desiredActive = false;
};

typedef std::shared_ptr<Displ> DisplP;

#endif //XLAYOUTDISPLAYS_DISPL_H
