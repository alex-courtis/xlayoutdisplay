#include <gtest/gtest.h>

#include "../src/Displ.h"

using namespace std;

class Displ_constructor : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    ModeP mode = make_shared<Mode>(0, 0, 0, 0);
    PosP pos = make_shared<Pos>(0, 0);
    EdidP edid;
    list <ModeP> modes = {shared_ptr<Mode>(mode)};
};


TEST_F(Displ_constructor, validActive) {
    EXPECT_NO_THROW(Displ("va", Displ::active, modes, mode, NULL, mode, pos, edid));
}

TEST_F(Displ_constructor, validConnected) {
    EXPECT_NO_THROW(Displ("vc", Displ::connected, modes, NULL, NULL, mode, NULL, edid));
}

TEST_F(Displ_constructor, validDisconnected) {
    EXPECT_NO_THROW(Displ("vd", Displ::disconnected, list<ModeP>(), NULL, NULL, NULL, NULL, edid));
}

TEST_F(Displ_constructor, activeMissingCurrentMode) {
    EXPECT_THROW(Displ("ia", Displ::active, modes, NULL, NULL, mode, pos, edid), invalid_argument);
}

TEST_F(Displ_constructor, activeMissingCurrentPos) {
    EXPECT_THROW(Displ("ia", Displ::active, modes, mode, NULL, mode, NULL, edid), invalid_argument);
}

TEST_F(Displ_constructor, activeMissingOptimalMode) {
    EXPECT_THROW(Displ("ia", Displ::active, modes, mode, NULL, NULL, pos, edid), invalid_argument);
}

TEST_F(Displ_constructor, activeEmptyModes) {
    EXPECT_THROW(Displ("ia", Displ::active, list<ModeP>(), mode, NULL, mode, pos, edid), invalid_argument);
}

TEST_F(Displ_constructor, connectedMissingOptimalMode) {
    EXPECT_THROW(Displ("ic", Displ::connected, modes, NULL, NULL, NULL, NULL, edid), invalid_argument);
}

TEST_F(Displ_constructor, connectedEmptyModes) {
    EXPECT_THROW(Displ("ic", Displ::connected, list<ModeP>(), NULL, NULL, mode, NULL, edid), invalid_argument);
}
