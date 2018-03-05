#include <gtest/gtest.h>

#include "../src/Edid.h"

using namespace std;

TEST(Edid_constructor, gteEdid) {
    const size_t len = EDID_MIN_LENGTH * 3 / 2;
    unsigned char val[len];
    memset(val, 1, len);

    Edid(val, len, "blargh");
}

TEST(Edid_constructor, ltEdid) {
    const size_t len = EDID_MIN_LENGTH - 1;
    unsigned char val[len];
    memset(val, 1, len);

    EXPECT_THROW(Edid(val, len, "blargh"), invalid_argument);
}

class Edid_measurements : public ::testing::Test {
protected:
    Edid_measurements() {
        memset(val, 1, EDID_MIN_LENGTH);
    }

    unsigned char val[EDID_MIN_LENGTH]{};
};

TEST_F(Edid_measurements, valid) {
    val[EDID_BYTE_MAX_CM_HORIZ] = 2;
    val[EDID_BYTE_MAX_CM_VERT] = 3;
    Edid edid = Edid(val, EDID_MIN_LENGTH, "valid");

    EXPECT_EQ(2, edid.maxCmHoriz());
    EXPECT_EQ(3, edid.maxCmVert());
    EXPECT_EQ(177, edid.dpiForMode(make_shared<Mode>(0, 123, 234, 0)));
}

TEST_F(Edid_measurements, zeroHoriz) {
    val[EDID_BYTE_MAX_CM_HORIZ] = 0;
    val[EDID_BYTE_MAX_CM_VERT] = 3;
    Edid edid = Edid(val, EDID_MIN_LENGTH, "zeroHoriz");

    EXPECT_EQ(0, edid.maxCmHoriz());
    EXPECT_EQ(3, edid.maxCmVert());
    EXPECT_EQ(0, edid.dpiForMode(make_shared<Mode>(0, 123, 234, 0)));
}

TEST_F(Edid_measurements, zeroVert) {
    val[EDID_BYTE_MAX_CM_HORIZ] = 2;
    val[EDID_BYTE_MAX_CM_VERT] = 0;
    Edid edid = Edid(val, EDID_MIN_LENGTH, "zeroVert");

    EXPECT_EQ(2, edid.maxCmHoriz());
    EXPECT_EQ(0, edid.maxCmVert());
    EXPECT_EQ(0, edid.dpiForMode(make_shared<Mode>(0, 123, 234, 0)));
}
