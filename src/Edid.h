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
#ifndef XLAYOUTDISPLAY_EDID_H
#define XLAYOUTDISPLAY_EDID_H

#include <memory>
#include "Mode.h"

#define EDID_MIN_LENGTH 128
#define EDID_BYTE_MAX_CM_HORIZ 0x15
#define EDID_BYTE_MAX_CM_VERT 0x16

class Edid {
public:
    // we only need the first 128 bytes of EDID - the basic structure
    // throws invalid_argument:
    //   when length < EDID_MIN_LENGTH
    Edid(const unsigned char *edid, size_t length, const char *name);

    virtual ~Edid();

    virtual const unsigned int maxCmHoriz() const;

    virtual const unsigned int maxCmVert() const;

    // nearest 12
    virtual const long dpiForMode(const std::shared_ptr<const Mode> &mode) const;

private:
    unsigned char *edid = nullptr;
};

#endif //XLAYOUTDISPLAY_EDID_H
