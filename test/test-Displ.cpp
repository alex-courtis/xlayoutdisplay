#include <gtest/gtest.h>

#include "../src/Displ.h"

using namespace std;

class DisplConstruction : public ::testing::Test {
protected:
    virtual void SetUp() {
    }

    ModeP mode = make_shared<Mode>(0, 0, 0, 0);
    PosP pos = make_shared<Pos>(0, 0);
    list <ModeP> modes = {shared_ptr<Mode>(mode)};
};


TEST_F(DisplConstruction, ConstructValidActive) {
    EXPECT_NO_THROW(Displ("va", Displ::active, modes, mode, NULL, mode, pos));
}

TEST_F(DisplConstruction, ConstructValidConnected) {
    EXPECT_NO_THROW(Displ("vc", Displ::connected, modes, NULL, NULL, mode, NULL));
}

TEST_F(DisplConstruction, ValidDisconnected) {
    EXPECT_NO_THROW(Displ("vd", Displ::disconnected, list<ModeP>(), NULL, NULL, NULL, NULL));
}

TEST_F(DisplConstruction, ActiveMissingCurrentMode) {
    EXPECT_THROW(Displ("ia", Displ::active, modes, NULL, NULL, mode, pos), invalid_argument);
}

TEST_F(DisplConstruction, ActiveMissingCurrentPos) {
    EXPECT_THROW(Displ("ia", Displ::active, modes, mode, NULL, mode, NULL), invalid_argument);
}

TEST_F(DisplConstruction, ActiveMissingOptimalMode) {
    EXPECT_THROW(Displ("ia", Displ::active, modes, mode, NULL, NULL, pos), invalid_argument);
}

TEST_F(DisplConstruction, ActiveEmptyModes) {
    EXPECT_THROW(Displ("ia", Displ::active, list<ModeP>(), mode, NULL, mode, pos), invalid_argument);
}

TEST_F(DisplConstruction, ConnectedMissingOptimalMode) {
    EXPECT_THROW(Displ("ic", Displ::connected, modes, NULL, NULL, NULL, NULL), invalid_argument);
}

TEST_F(DisplConstruction, ConnectedEmptyModes) {
    EXPECT_THROW(Displ("ic", Displ::connected, list<ModeP>(), NULL, NULL, mode, NULL), invalid_argument);
}
