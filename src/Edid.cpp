/*
   Copyright 2018 Alexander Courtis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "Edid.h"

#include <cstring>
#include <cmath>
#include <system_error>

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

const unsigned int Edid::maxCmHoriz() const {
    return edid[EDID_BYTE_MAX_CM_HORIZ];
}

const unsigned int Edid::maxCmVert() const {
    return edid[EDID_BYTE_MAX_CM_VERT];
}

const long Edid::dpiForMode(const std::shared_ptr<const Mode> &mode) const {
    if (maxCmVert() == 0 || maxCmHoriz() == 0) {
        return 0;
    }
    double dpiHoriz = mode->width * INCHES_PER_CM / maxCmHoriz();
    double dpiVert = mode->height * INCHES_PER_CM / maxCmVert();

    // nearest 12 dpi
    return lround((dpiHoriz + dpiVert) / 24) * 12;
}

