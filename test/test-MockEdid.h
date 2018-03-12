#ifndef XLAYOUTDISPLAY_TEST_MOCKEDID_H
#define XLAYOUTDISPLAY_TEST_MOCKEDID_H

#include <gmock/gmock.h>

#include "../src/Edid.h"

class MockEdid : public Edid {
public:
    MockEdid() : Edid(
            reinterpret_cast<const unsigned char *>("01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF01234567890ABCDEF"),
            EDID_MIN_LENGTH, "MockEdid") {};

    MOCK_CONST_METHOD0(maxCmHoriz, const unsigned int());

    MOCK_CONST_METHOD0(maxCmVert, const unsigned int());

    MOCK_CONST_METHOD1(dpiForMode, const long(const std::shared_ptr<Mode> &mode));
};

#endif //XLAYOUTDISPLAY_TEST_MOCKEDID_H
