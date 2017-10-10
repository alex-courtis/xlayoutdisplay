#include <gtest/gtest.h>

#include "../src/Mode.h"

using namespace std;

TEST(Mode_order, width) {
    EXPECT_TRUE(Mode(0, 1, 2, 2) < Mode(0, 2, 1, 1));
}

TEST(Mode_order, height) {
    EXPECT_TRUE(Mode(0, 1, 1, 2) < Mode(0, 1, 2, 1));
}

TEST(Mode_order, refresh) {
    EXPECT_TRUE(Mode(0, 1, 1, 1) < Mode(0, 1, 1, 2));
}
