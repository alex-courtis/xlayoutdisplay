#include <gtest/gtest.h>

#include "../src/Mode.h"

using namespace std;

class Mode_constructor : public ::testing::Test {
protected:
    virtual void SetUp() {
        resources.nmode = 3;
        modeInfos[0].id = 10;
        modeInfos[1].id = 11;
        modeInfos[2].id = 12;
        resources.modes = &modeInfos[0];
    }

    XRRScreenResources resources;
    XRRModeInfo modeInfos[3];
};

TEST_F(Mode_constructor, valid) {
    Mode::fromXRR(11, &resources);
}

TEST_F(Mode_constructor, modeNotPresent) {
    EXPECT_THROW(Mode::fromXRR(13, &resources), invalid_argument);
}

TEST_F(Mode_constructor, resourcesNotPresent) {
    EXPECT_THROW(Mode::fromXRR(11, nullptr), invalid_argument);
}


TEST(Mode_order, width) {
    EXPECT_TRUE(Mode(0, 1, 2, 2) < Mode(0, 2, 1, 1));
}

TEST(Mode_order, height) {
    EXPECT_TRUE(Mode(0, 1, 1, 2) < Mode(0, 1, 2, 1));
}

TEST(Mode_order, refresh) {
    EXPECT_TRUE(Mode(0, 1, 1, 1) < Mode(0, 1, 1, 2));
}
