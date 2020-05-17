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
#ifndef XLAYOUTDISPLAY_TEST_MOCKEDID_H
#define XLAYOUTDISPLAY_TEST_MOCKEDID_H

#include <gmock/gmock.h>

#include "../src/Edid.h"

class MockEdid : public Edid {
public:
    MockEdid() : Edid(
            reinterpret_cast<const unsigned char *>("01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF"),
            EDID_MIN_LENGTH, "MockEdid") {};

    MOCK_CONST_METHOD0(maxCmHoriz, unsigned int());

    MOCK_CONST_METHOD0(maxCmVert, unsigned int());

    MOCK_CONST_METHOD1(dpiForMode, long(const std::shared_ptr<const Mode> &mode));
};

#endif //XLAYOUTDISPLAY_TEST_MOCKEDID_H
