#include <gtest/gtest.h>

#include "../src/Mode.h"

using namespace std;

class TestMode : public ::testing::Test {
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

TEST_F(TestMode, ConstructValid) {
    EXPECT_NO_THROW(Mode::fromXRR(11, &resources));
}

TEST_F(TestMode, ConstructModeNotPresent) {
    EXPECT_THROW(Mode::fromXRR(13, &resources), invalid_argument);
}

TEST_F(TestMode, ConstructResourcesNotPresent) {
    EXPECT_THROW(Mode::fromXRR(11, NULL), invalid_argument);
}
