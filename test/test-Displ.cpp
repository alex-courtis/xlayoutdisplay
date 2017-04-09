#include <gtest/gtest.h>

#include "../src/Displ.h"

using namespace std;

ModeP mode = make_shared<Mode>(0, 0, 0, 0);
PosP pos = make_shared<Pos>(0, 0);
list<ModeP> emptyModes;
list<ModeP> nonEmptyModes = { shared_ptr<Mode>(mode) };

TEST(TestDispl, ConstructValidActive) {
    EXPECT_NO_THROW(Displ("va", Displ::active, nonEmptyModes, mode, NULL, mode, pos));
}

TEST(TestDispl, ConstructValidConnected) {
    EXPECT_NO_THROW(Displ("vc", Displ::connected, nonEmptyModes, NULL, NULL, mode, NULL));
}

TEST(TestDispl, ConstructValidDisconnected) {
    EXPECT_NO_THROW(Displ("vd", Displ::disconnected, emptyModes, NULL, NULL, NULL, NULL));
}

TEST(TestDispl, ConstructActiveMissingCurrentMode) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, NULL, NULL, mode, pos), invalid_argument);
}

TEST(TestDispl, ConstructActiveMissingCurrentPos) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, mode, NULL, mode, NULL), invalid_argument);
}

TEST(TestDispl, ConstructActiveMissingOptimalMode) {
    EXPECT_THROW(Displ("ia", Displ::active, nonEmptyModes, mode, NULL, NULL, pos), invalid_argument);
}

TEST(TestDispl, ConstructActiveEmptyModes) {
    EXPECT_THROW(Displ("ia", Displ::active, emptyModes, mode, NULL, mode, pos), invalid_argument);
}

TEST(TestDispl, ConstructConnectedMissingOptimalMode) {
    EXPECT_THROW(Displ("ic", Displ::connected, nonEmptyModes, NULL, NULL, NULL, NULL), invalid_argument);
}

TEST(TestDispl, ConstructConnectedEmptyModes) {
    EXPECT_THROW(Displ("ic", Displ::connected, emptyModes, NULL, NULL, mode, NULL), invalid_argument);
}
