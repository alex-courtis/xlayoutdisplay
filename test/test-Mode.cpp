#include <gtest/gtest.h>

#include "../src/Mode.h"

using namespace std;

class ModeConstruction : public ::testing::Test {
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

TEST_F(ModeConstruction, Valid) {
    EXPECT_NO_THROW(Mode::fromXRR(11, &resources));
}

TEST_F(ModeConstruction, ModeNotPresent) {
    EXPECT_THROW(Mode::fromXRR(13, &resources), invalid_argument);
}

TEST_F(ModeConstruction, ResourcesNotPresent) {
    EXPECT_THROW(Mode::fromXRR(11, NULL), invalid_argument);
}


TEST(ModeOrder, Width) {
    ASSERT_TRUE(Mode(0, 1, 2, 2) < Mode(0, 2, 1, 1));
}

TEST(ModeOrder, Height) {
    ASSERT_TRUE(Mode(0, 1, 1, 2) < Mode(0, 1, 2, 1));
}

TEST(ModeOrder, Refresh) {
    ASSERT_TRUE(Mode(0, 1, 1, 1) < Mode(0, 1, 1, 2));
}
