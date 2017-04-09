#ifndef XLAYOUTDISPLAYS_MODE_H
#define XLAYOUTDISPLAYS_MODE_H

#include <memory>
#include <X11/extensions/Xrandr.h>

// a mode that may be used by an Xrandr display
class Mode {
public:
    Mode(const RRMode &rrMode, const unsigned int &width, const unsigned int &height, const unsigned int &refresh) :
            rrMode(rrMode), width(width), height(height), refresh(refresh) {
    }

    virtual ~Mode() {
    }

    // pseudo-constructor that validates prior to invoking constructor
    static Mode *fromXRR(const RRMode id, const XRRScreenResources *resources);

    // order by width, refresh, height in descending order
    bool operator<(const Mode &o);

    const RRMode rrMode;
    const unsigned int width;
    const unsigned int height;
    const unsigned int refresh;
};

typedef std::shared_ptr<Mode> ModeP;

#endif //XLAYOUTDISPLAYS_MODE_H
