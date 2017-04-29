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
    EXPECT_NO_THROW(Displ("va", Displ::active, modes, mode1, NULL, pos, edid));
}

TEST_F(Displ_constructor, validConnected) {
    EXPECT_NO_THROW(Displ("vc", Displ::connected, modes, NULL, NULL, NULL, edid));
}

TEST_F(Displ_constructor, validDisconnected) {
    EXPECT_NO_THROW(Displ("vd", Displ::disconnected, list<ModeP>(), NULL, NULL, NULL, edid));
}

TEST_F(Displ_constructor, activeMissingCurrentMode) {
    EXPECT_THROW(Displ("ia", Displ::active, modes, NULL, NULL, pos, edid), invalid_argument);
}

TEST_F(Displ_constructor, activeMissingCurrentPos) {
    EXPECT_THROW(Displ("ia", Displ::active, modes, mode1, NULL, NULL, edid), invalid_argument);
}

TEST_F(Displ_constructor, activeEmptyModes) {
    EXPECT_THROW(Displ("ia", Displ::active, list<ModeP>(), mode1, NULL, pos, edid), invalid_argument);
}

TEST_F(Displ_constructor, connectedEmptyModes) {
    EXPECT_THROW(Displ("ic", Displ::connected, list<ModeP>(), NULL, NULL, NULL, edid), invalid_argument);
}

TEST_F(Displ_constructor, optimalMissing) {
    Displ displ = Displ("om", Displ::disconnected, list<ModeP>(), NULL, NULL, NULL, edid);
    EXPECT_FALSE(displ.optimalMode);
}

TEST_F(Displ_constructor, optimalFirst) {
    Displ displ = Displ("of", Displ::disconnected, modes, NULL, NULL, NULL, edid);
    EXPECT_EQ(mode2, displ.optimalMode);
}

TEST_F(Displ_constructor, optimalPreferred) {
    Displ displ = Displ("op", Displ::disconnected, modes, NULL, mode1, NULL, edid);
    EXPECT_EQ(mode1, displ.optimalMode);
}

TEST_F(Displ_constructor, optimalPreferredHigherRefresh) {
    ModeP mode3 = make_shared<Mode>(4, 5, 6, 70);
    modes.push_front(mode3);
    Displ displ = Displ("oph", Displ::disconnected, modes, NULL, mode2, NULL, edid);
    EXPECT_EQ(mode3, displ.optimalMode);
}

TEST(Displ_setDesiredActive, optimalPresent) {
    Displ displ = Displ("op", Displ::disconnected, {make_shared<Mode>(0, 0, 0, 0)}, NULL, NULL, NULL, NULL);
    EXPECT_NO_THROW(displ.setDesiredActive());
}

TEST(Displ_setDesiredActive, missingOptimal) {
    Displ displ = Displ("mo", Displ::disconnected, list<ModeP>(), NULL, NULL, NULL, NULL);
    EXPECT_THROW(displ.setDesiredActive(), invalid_argument);
}