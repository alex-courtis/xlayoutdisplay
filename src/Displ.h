#ifndef XLAYOUTDISPLAYS_DISPL_H
#define XLAYOUTDISPLAYS_DISPL_H

#include "Mode.h"
#include "Pos.h"
#include "Edid.h"
#include "Monitors.h"

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
    //   active/connected must have: empty or currentMode/preferredMode in modes
    // modes will be ordered descending
    // optimalMode will be set to highest refresh preferredMode, then highest mode, then empty
    // TODO: maybe a builder pattern
    Displ(const std::string &name,
          const State &state,
          const std::list<std::shared_ptr<Mode>> &modes,
          const std::shared_ptr<Mode> &currentMode,
          const std::shared_ptr<Mode> &preferredMode,
          const std::shared_ptr<Pos> &currentPos,
          const std::shared_ptr<Edid> &edid);

    const std::string name;
    const State state;
    const std::list<std::shared_ptr<Mode>> modes;
    const std::shared_ptr<Mode> currentMode;
    const std::shared_ptr<Mode> preferredMode;
    const std::shared_ptr<Mode> optimalMode;
    const std::shared_ptr<Pos> currentPos;
    const std::shared_ptr<Edid> edid;

    std::shared_ptr<Pos> desiredPos;

    // desiredMode must be in modes
    void desiredMode(const std::shared_ptr<Mode> &desiredMode);

    const std::shared_ptr<Mode> &desiredMode() const;

    // optimalMode must be present
    void desiredActive(bool desiredActive);

    bool desiredActive() const;

private:
    std::shared_ptr<Mode> _desiredMode;
    bool _desiredActive = false;
};


#endif //XLAYOUTDISPLAYS_DISPL_H
