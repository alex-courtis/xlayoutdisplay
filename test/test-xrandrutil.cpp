#include <gtest/gtest.h>

#include "../src/xrandrrutil.h"

TEST(RefreshFromModeId, NullOK) {
    EXPECT_EQ(0, refreshFromModeInfo(NULL));
}

