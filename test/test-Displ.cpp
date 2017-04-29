#include <gtest/gtest.h>

#include "../src/Displ.h"

using namespace std;

class Displ_constructor : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    ModeP mode1 = make_shared<Mode>(0, 1, 2, 3);
    ModeP mode2 = make_shared<Mode>(4, 5, 6, 7);

    PosP pos = make_shared<Pos>(0, 0);
    EdidP edid;
    list <ModeP> modes = {mode1, mode2};
};


TEST_F(Displ_constructor, validActive) {
    Displ("validActive", Displ::active, modes, mode1, NULL, pos, edid);
}

TEST_F(Displ_constructor, validConnected) {
    Displ("validConnected", Displ::connected, modes, NULL, NULL, NULL, edid);
}

TEST_F(Displ_constructor, validDisconnected) {
    Displ("validDisconnected", Displ::disconnected, list<ModeP>(), NULL, NULL, NULL, edid);
}

TEST_F(Displ_constructor, activeMissingCurrentMode) {
    EXPECT_THROW(Displ("activeMissingCurrentMode", Displ::active, modes, NULL, NULL, pos, edid), invalid_argument);
}

TEST_F(Displ_constructor, activeMissingCurrentPos) {
    EXPECT_THROW(Displ("activeMissingCurrentPos", Displ::active, modes, mode1, NULL, NULL, edid), invalid_argument);
}

TEST_F(Displ_constructor, activeEmptyModes) {
    EXPECT_THROW(Displ("activeEmptyModes", Displ::active, list<ModeP>(), mode1, NULL, pos, edid), invalid_argument);
}

TEST_F(Displ_constructor, connectedEmptyModes) {
    EXPECT_THROW(Displ("connectedEmptyModes", Displ::connected, list<ModeP>(), NULL, NULL, NULL, edid), invalid_argument);
}

TEST_F(Displ_constructor, optimalMissing) {
    Displ displ = Displ("optimalMissing", Displ::disconnected, list<ModeP>(), NULL, NULL, NULL, edid);
    EXPECT_FALSE(displ.getOptimalMode());
}

TEST_F(Displ_constructor, optimalFirst) {
    Displ displ = Displ("optimalFirst", Displ::disconnected, modes, NULL, NULL, NULL, edid);
    EXPECT_EQ(mode2, displ.getOptimalMode());
}

TEST_F(Displ_constructor, optimalPreferred) {
    Displ displ = Displ("optimalPreferred", Displ::disconnected, modes, NULL, mode1, NULL, edid);
    EXPECT_EQ(mode1, displ.getOptimalMode());
}

TEST_F(Displ_constructor, optimalPreferredHigherRefresh) {
    ModeP mode3 = make_shared<Mode>(4, 5, 6, 70);
    modes.push_front(mode3);
    Displ displ = Displ("optimalPreferredHigherRefresh", Displ::disconnected, modes, NULL, mode2, NULL, edid);
    EXPECT_EQ(mode3, displ.getOptimalMode());
}

TEST_F(Displ_constructor, currentNotInModes) {
    EXPECT_THROW(Displ("currentNotInModes", Displ::connected, modes, make_shared<Mode>(1, 2, 3, 4), NULL, NULL, edid), invalid_argument);
}

TEST_F(Displ_constructor, preferredNotInModes) {
    EXPECT_THROW(Displ("preferredNotInModes", Displ::connected, modes, NULL, make_shared<Mode>(5, 6, 7, 8), NULL, edid), invalid_argument);
}

TEST_F(Displ_constructor, disconnectedNotInModes) {
    Displ("disconnectedNotInModes", Displ::disconnected, list<ModeP>(), make_shared<Mode>(11, 12, 13, 14), make_shared<Mode>(15, 16, 17, 18), NULL, edid);
}


TEST(Displ_setDesiredActive, optimalPresent) {
    Displ displ = Displ("optimalPresent", Displ::disconnected, {make_shared<Mode>(9, 0, 9, 8)}, NULL, NULL, NULL, NULL);
    displ.setDesiredActive();
}

TEST(Displ_setDesiredActive, missingOptimal) {
    Displ displ = Displ("missingOptimal", Displ::disconnected, list<ModeP>(), NULL, NULL, NULL, NULL);
    EXPECT_THROW(displ.setDesiredActive(), invalid_argument);
}