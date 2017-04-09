#include <gtest/gtest.h>

#include "../src/xrandrrutil.h"

TEST(RefreshFromModeId, Null) {
    EXPECT_EQ(0, refreshFromModeInfo(NULL));
}

