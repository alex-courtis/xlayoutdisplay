#include <gtest/gtest.h>

#include "../src/xrandrrutil.h"

TEST(xrandrutil_refreshFromModeInfo, nullOK) {
    EXPECT_EQ(0, refreshFromModeInfo(NULL));
}
