#include <gtest/gtest.h>

#include "../src/Displ.h"

using namespace std;

ModeP mode = make_shared<Mode>(0, 0, 0, 0);
PosP pos = make_shared<Pos>(0, 0);
list<ModeP> emptyModes;
list<ModeP> nonEmptyModes = { shared_ptr<Mode>(mode) };

TEST(Displ, ConstructValidActive) {
    Displ("va", Displ::active, nonEmptyModes, mode, NULL, mode, pos);
}

TEST(Displ, ConstructValidConnected) {
    Displ("vc", Displ::connected, nonEmptyModes, NULL, NULL, mode, NULL);
}

TEST(Displ, ConstructValidDisconnected) {
    Displ("vd", Displ::disconnected, emptyModes, NULL, NULL, NULL, NULL);
}

TEST(Displ, ConstructActiveMissingCurrentMode) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, NULL, NULL, mode, pos), invalid_argument);
}

TEST(Displ, ConstructActiveMissingCurrentPos) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, mode, NULL, mode, NULL), invalid_argument);
}

TEST(Displ, ConstructActiveMissingOptimalMode) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, mode, NULL, NULL, pos), invalid_argument);
}

TEST(Displ, ConstructActiveEmptyModes) {
    EXPECT_THROW(Displ("ia", Displ::active, emptyModes, mode, NULL, mode, pos), invalid_argument);
}

TEST(Displ, ConstructConnectedMissingOptimalMode) {
    EXPECT_THROW(Displ("ic", Displ::connected, nonEmptyModes, NULL, NULL, NULL, NULL), invalid_argument);
}

TEST(Displ, ConstructConnectedEmptyModes) {
    EXPECT_THROW(Displ("ic", Displ::connected, emptyModes, NULL, NULL, mode, NULL), invalid_argument);
}
