#include <gtest/gtest.h>

#include "../src/Displ.h"

using namespace std;

ModeP mode = make_shared<Mode>(0, 0, 0, 0);
PosP pos = make_shared<Pos>(0, 0);
list<ModeP> emptyModes;
list<ModeP> nonEmptyModes = { shared_ptr<Mode>(mode) };

TEST(DisplConstruction, ConstructValidActive) {
    EXPECT_NO_THROW(Displ("va", Displ::active, nonEmptyModes, mode, NULL, mode, pos));
}

TEST(DisplConstruction, ConstructValidConnected) {
    EXPECT_NO_THROW(Displ("vc", Displ::connected, nonEmptyModes, NULL, NULL, mode, NULL));
}

TEST(DisplConstruction, ValidDisconnected) {
    EXPECT_NO_THROW(Displ("vd", Displ::disconnected, emptyModes, NULL, NULL, NULL, NULL));
}

TEST(DisplConstruction, ActiveMissingCurrentMode) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, NULL, NULL, mode, pos), invalid_argument);
}

TEST(DisplConstruction, ActiveMissingCurrentPos) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, mode, NULL, mode, NULL), invalid_argument);
}

TEST(DisplConstruction, ActiveMissingOptimalMode) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, mode, NULL, NULL, pos), invalid_argument);
}

TEST(DisplConstruction, ActiveEmptyModes) {
    EXPECT_THROW(Displ("ia", Displ::active, emptyModes, mode, NULL, mode, pos), invalid_argument);
}

TEST(DisplConstruction, ConnectedMissingOptimalMode) {
    EXPECT_THROW(Displ("ic", Displ::connected, nonEmptyModes, NULL, NULL, NULL, NULL), invalid_argument);
}

TEST(DisplConstruction, ConnectedEmptyModes) {
    EXPECT_THROW(Displ("ic", Displ::connected, emptyModes, NULL, NULL, mode, NULL), invalid_argument);
}
