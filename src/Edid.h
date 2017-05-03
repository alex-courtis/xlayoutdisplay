#ifndef XLAYOUTDISPLAYS_EDID_H
#define XLAYOUTDISPLAYS_EDID_H

#include <memory>
#include "Mode.h"

#define DPI_STEP 24 // use quarters to prevent blurring, see https://wiki.archlinux.org/index.php/xorg#Setting_DPI_manually

class Edid {
public:

    // throws invalid_argument:
    //   length > EDID_MIN_LENGTH
    Edid(const unsigned char *edid, const size_t length, const char *name);

    virtual ~Edid();

    virtual int maxCmHoriz() const;

    virtual int maxCmVert() const;

    virtual double dpiForMode(const ModeP &mode) const;

    virtual int closestDpiForMode(const ModeP &mode) const;

private:
    unsigned char *edid;
};

typedef std::shared_ptr<Edid> EdidP;

#define EDID_MIN_LENGTH 128

#define EDID_MAX_CM_HORIZ 0x15
#define EDID_MAX_CM_VERT 0x16

#define INCHES_PER_CM 2.54 // apparently this is exact

#endif //XLAYOUTDISPLAYS_EDID_H
