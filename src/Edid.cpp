#include "Edid.h"

#include "edidConsts.h"

#include <string.h>

using namespace std;

EdidImpl::EdidImpl(const unsigned char *edid, const size_t length, const char *name) {
    if (length < EDID_LENGTH)
        throw invalid_argument(string(name) + " has Edid size " + to_string(length) + ", expected at least " + to_string(EDID_LENGTH));

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
