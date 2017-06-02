#ifndef XLAYOUTDISPLAYS_DISPL_H
#define XLAYOUTDISPLAYS_DISPL_H

#include "Mode.h"
#include "Pos.h"
#include "Edid.h"

#include <memory>
#include <list>

#define DEFAULT_DPI 96

class Displ;

typedef std::shared_ptr<Displ> DisplP;

// reorder displs putting those whose names match order at the front, case insensitive
void orderDispls(std::list<DisplP> &displs, const std::list<std::string> &order);

// mark displays that should be activated and set the one and only primary
void activateDispls(std::list<DisplP> &displs, const std::string &primary);

// arrange desiredActive displays left to right at optimal mode; will mutate contents
void ltrDispls(std::list<DisplP> &displs);

// arrange desiredActive displays so that they all mirror at lowest common mode; will mutate contents
// throws runtime_error if no common mode found
void mirrorDispls(std::list<DisplP> &displs);

std::string calculateDpi(std::list<DisplP> &displs);

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
    Displ(const std::string &name, const State &state, const std::list<ModeP> &modes, const ModeP &currentMode,
          const ModeP &preferredMode, const PosP &currentPos, const EdidP edid);

    virtual ~Displ() {
    }

    const std::string name;
    const State state;
    const std::list<ModeP> modes;
    const ModeP currentMode;
    const ModeP preferredMode;
    const ModeP optimalMode;
    const PosP currentPos;
    const EdidP edid;

    PosP desiredPos;

    static std::shared_ptr<Displ> desiredPrimary;

    // defaults to DEFAULT_DPI
    static unsigned int desiredDpi;

    // desiredMode must be in modes
    void desiredMode(const ModeP &desiredMode);

    const ModeP &desiredMode() const;

    // optimalMode must be present
    void desiredActive(const bool desiredActive);

    bool desiredActive() const;

private:
    ModeP _desiredMode;
    bool _desiredActive = false;
};


#endif //XLAYOUTDISPLAYS_DISPL_H
