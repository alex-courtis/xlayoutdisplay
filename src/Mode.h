#ifndef XLAYOUTDISPLAYS_MODE_H
#define XLAYOUTDISPLAYS_MODE_H

#include <memory>
#include <X11/extensions/Xrandr.h>

// a mode that may be used by an Xrandr display
class Mode {
public:
    Mode(const RRMode &rrMode,
         const unsigned int &width,
         const unsigned int &height,
         const unsigned int &refresh) :
            rrMode(rrMode),
            width(width),
            height(height),
            refresh(refresh) {
    }

    // throws invalid_argument:
    //   null resources
    //   id not found in resources
    static Mode *fromXRR(RRMode id, const XRRScreenResources *resources);

    // order by width, height, refresh
    const bool operator<(const Mode &o) const;

    const RRMode rrMode;
    const unsigned int width;
    const unsigned int height;
    const unsigned int refresh;
};

#endif //XLAYOUTDISPLAYS_MODE_H
