#include "Edid.h"

#include <cstring>
#include <cmath>

using namespace std;

#define INCHES_PER_CM   2.54    // apparently this is exact

Edid::Edid(const unsigned char *edid, const size_t length, const char *name) {
    if (length < EDID_MIN_LENGTH)
        throw invalid_argument(string(name) + " has Edid size " + to_string(length) + ", expected at least " +
                               to_string(EDID_MIN_LENGTH));

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

// TODO test
const long Edid::dpiForMode(const std::shared_ptr<Mode> &mode) const {
    if (maxCmVert() == 0 || maxCmHoriz() == 0) {
        return 0;
    }
    double dpiHoriz = mode->width * INCHES_PER_CM / maxCmHoriz();
    double dpiVert = mode->height * INCHES_PER_CM / maxCmVert();
    return lround((dpiHoriz + dpiVert) / 2);
}