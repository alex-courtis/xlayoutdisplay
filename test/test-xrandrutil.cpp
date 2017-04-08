#include <gtest/gtest.h>

#include "../src/xrandrrutil.h"

TEST(RefreshFromModeId, Null) {
    EXPECT_EQ(0, refreshFromModeInfo(NULL));
}


class ModeInfoFromId : public ::testing::Test {
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

TEST_F(ModeInfoFromId, ResourcesMissing) {
    EXPECT_EQ(NULL, modeInfoFromId(0, NULL));
}

TEST_F(ModeInfoFromId, ModeInfoPresent) {
    EXPECT_EQ(&modeInfos[1], modeInfoFromId(11, &resources));
}

TEST_F(ModeInfoFromId, ModeInfoNotPresent) {
    EXPECT_EQ(NULL, modeInfoFromId(999, &resources));
}
