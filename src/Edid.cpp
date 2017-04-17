#include "Edid.h"

#include <string.h>

using namespace std;

EdidImpl::EdidImpl(const unsigned char *edid, const size_t length, const char *name) {
    if (length < EDID_MIN_LENGTH)
        throw invalid_argument(string(name) + " has Edid size " + to_string(length) + ", expected at least " + to_string(EDID_MIN_LENGTH));

    this->edid = (unsigned char *) malloc(length);
    memcpy(this->edid, edid, length);
}

EdidImpl::~EdidImpl() {
    free(edid);
}

int EdidImpl::maxCmHoriz() const {
    return edid[EDID_MAX_CM_HORIZ];
}

int EdidImpl::maxCmVert() const {
    return edid[EDID_MAX_CM_VERT];
}

double EdidImpl::dpiForMode(const ModeP &mode) const {
    double dpiHoriz = mode->width * INCHES_PER_CM / maxCmHoriz();
    double dpiVert = mode->height * INCHES_PER_CM / maxCmVert();
    return (dpiHoriz + dpiVert) / 2;
}

int EdidImpl::closestDpiForMode(const ModeP &mode) const {
    return (int)((dpiForMode(mode) + DPI_STEP / 2) / 24) * 24;
}
