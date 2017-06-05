#include "Edid.h"

#include <string.h>

using namespace std;

#define DPI_STEP        24      // use quarters to prevent blurring, see https://wiki.archlinux.org/index.php/xorg#Setting_DPI_manually
#define INCHES_PER_CM   2.54    // apparently this is exact

Edid::Edid(const unsigned char *edid, const size_t length, const char *name) {
    if (length < EDID_MIN_LENGTH)
        throw invalid_argument(string(name) + " has Edid size " + to_string(length) + ", expected at least " + to_string(EDID_MIN_LENGTH));

    this->edid = (unsigned char *) malloc(length);
    memcpy(this->edid, edid, length);
}

Edid::~Edid() {
    free(edid);
}

const int Edid::maxCmHoriz() const {
    return edid[EDID_BYTE_MAX_CM_HORIZ];
}

const int Edid::maxCmVert() const {
    return edid[EDID_BYTE_MAX_CM_VERT];
}

const double Edid::dpiForMode(const ModeP &mode) const {
    if (maxCmVert() == 0 || maxCmHoriz() == 0) {
        return 0;
    }
    double dpiHoriz = mode->width * INCHES_PER_CM / maxCmHoriz();
    double dpiVert = mode->height * INCHES_PER_CM / maxCmVert();
    return (dpiHoriz + dpiVert) / 2;
}

const unsigned int Edid::closestDpiForMode(const ModeP &mode) const {
    return (unsigned int)((dpiForMode(mode) + DPI_STEP / 2) / DPI_STEP) * DPI_STEP;
}
