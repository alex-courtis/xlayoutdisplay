#ifndef XLAYOUTDISPLAYS_MODE_H
#define XLAYOUTDISPLAYS_MODE_H

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

    // order by width, height, refresh
    const bool operator<(const Mode &o) const;

    const RRMode rrMode;
    const unsigned int width;
    const unsigned int height;
    const unsigned int refresh;
};

#endif //XLAYOUTDISPLAYS_MODE_H
