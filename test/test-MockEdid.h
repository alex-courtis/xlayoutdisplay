#include <gmock/gmock.h>

#include "../src/Edid.h"

#ifndef XLAYOUTDISPLAYS_TEST_MOCKEDID_H
#define XLAYOUTDISPLAYS_TEST_MOCKEDID_H

class MockEdid : public Edid {
public:
    MOCK_CONST_METHOD0(maxCmHoriz, const unsigned int());

    MOCK_CONST_METHOD0(maxCmVert, const unsigned int());

    MOCK_CONST_METHOD1(dpiForMode, const long(
    const std::shared_ptr<Mode> &mode));
};

#endif //XLAYOUTDISPLAYS_TEST_MOCKEDID_H
