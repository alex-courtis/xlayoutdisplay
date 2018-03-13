#ifndef XLAYOUTDISPLAY_DISPL_H
#define XLAYOUTDISPLAY_DISPL_H

#include "Mode.h"
#include "Pos.h"
#include "Edid.h"
#include "Monitors.h"

#include <memory>
#include <list>

// a single Xrandr output
class Output {
public:
    enum State {
        active, connected, disconnected
    };

    // throws invalid_argument:
    //   active must have: currentMode, currentPos, modes
    //   connected must have: modes
    //   active/connected must have: empty or currentMode/preferredMode in modes
    // modes will be ordered descending
    // optimalMode will be set to highest refresh preferredMode, then highest mode, then empty
    Output(const std::string &name,
           const State &state,
           const std::list<std::shared_ptr<const Mode>> &modes,
           const std::shared_ptr<const Mode> &currentMode,
           const std::shared_ptr<const Mode> &preferredMode,
           const std::shared_ptr<const Pos> &currentPos,
           const std::shared_ptr<const Edid> &edid);

    const std::string name;
    const State state;
    const std::list<std::shared_ptr<const Mode>> modes;
    const std::shared_ptr<const Mode> currentMode;
    const std::shared_ptr<const Mode> preferredMode;
    const std::shared_ptr<const Mode> optimalMode;
    const std::shared_ptr<const Pos> currentPos;
    const std::shared_ptr<const Edid> edid;

    bool desiredActive = false;
    std::shared_ptr<const Mode> desiredMode;
    std::shared_ptr<const Pos> desiredPos;
};


#endif //XLAYOUTDISPLAY_DISPL_H
