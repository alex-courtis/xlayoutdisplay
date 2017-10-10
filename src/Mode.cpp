#include "Mode.h"

const bool Mode::operator<(const Mode &o) const {
    if (width == o.width)
        if (height == o.height)
            return refresh < o.refresh;
        else
            return height < o.height;
    else
        return width < o.width;
}
