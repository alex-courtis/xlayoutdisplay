#ifndef XLAYOUTDISPLAY_EDID_H
#define XLAYOUTDISPLAY_EDID_H

#include <memory>
#include "Mode.h"

#define EDID_MIN_LENGTH 128
#define EDID_BYTE_MAX_CM_HORIZ 0x15
#define EDID_BYTE_MAX_CM_VERT 0x16

class Edid {
public:
    Edid() = default;

    // we only need the first 128 bytes of EDID - the basic structure
    // throws invalid_argument:
    //   when length < EDID_MIN_LENGTH
    Edid(const unsigned char *edid, size_t length, const char *name);

    virtual ~Edid();

    virtual const unsigned int maxCmHoriz() const;

    virtual const unsigned int maxCmVert() const;

    virtual const long dpiForMode(const std::shared_ptr<Mode> &mode) const;

private:
    unsigned char *edid = nullptr;
};

#endif //XLAYOUTDISPLAY_EDID_H
