#ifndef XLAYOUTDISPLAYS_EDID_H
#define XLAYOUTDISPLAYS_EDID_H

#include <memory>
#include "Mode.h"

// Edid info from monitor - assumes 1.4 spec
class Edid {
public:
    virtual ~Edid() {};

    virtual int maxCmHoriz() const = 0;

    virtual int maxCmVert() const = 0;

    // average horiz/vert DPI for mode
    virtual double dpiForMode(const ModeP &mode) const = 0;

    // closest DPI_STEP for dpiForMode
    virtual int closestDpiForMode(const ModeP &mode) const = 0;
};

typedef std::shared_ptr<Edid> EdidP;

class EdidImpl : public Edid {
public:

    // throws invalid_argument:
    //   length > EDID_MIN_LENGTH
    EdidImpl(const unsigned char *edid, const size_t length, const char *name);

    virtual ~EdidImpl();

    int maxCmHoriz() const;

    int maxCmVert() const;

    double dpiForMode(const ModeP &mode) const;

    int closestDpiForMode(const ModeP &mode) const;

private:
    unsigned char *edid;
};

#define EDID_MIN_LENGTH 128

#define EDID_MAX_CM_HORIZ 0x15
#define EDID_MAX_CM_VERT 0x16

#define DPI_STEP 24

#define INCHES_PER_CM 2.54 // apparently this is exact

#endif //XLAYOUTDISPLAYS_EDID_H
