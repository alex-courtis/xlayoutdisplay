#ifndef XLAYOUTDISPLAYS_EDID_H
#define XLAYOUTDISPLAYS_EDID_H

#include <memory>
#include "Mode.h"

#define EDID_MIN_LENGTH 128
#define EDID_BYTE_MAX_CM_HORIZ 0x15
#define EDID_BYTE_MAX_CM_VERT 0x16

class Edid {
public:
    Edid() {};

    // throws invalid_argument when length > EDID_MIN_LENGTH
    Edid(const unsigned char *edid, const size_t length, const char *name);

    virtual ~Edid();

    virtual const int maxCmHoriz() const;

    virtual const int maxCmVert() const;

    virtual const double dpiForMode(const ModeP &mode) const;

    virtual const unsigned int closestDpiForMode(const ModeP &mode) const;

private:
    unsigned char *edid;
};

typedef std::shared_ptr<Edid> EdidP;

#endif //XLAYOUTDISPLAYS_EDID_H
