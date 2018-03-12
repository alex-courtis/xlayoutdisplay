#include <gtest/gtest.h>

#include "../src/Output.h"

using namespace std;

class Output_test : public ::testing::Test {
protected:
    std::shared_ptr<Mode> mode1 = make_shared<Mode>(0, 1, 2, 3);
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(4, 5, 6, 7);
    std::shared_ptr<Mode> modeInexistent = make_shared<Mode>(5, 4, 3, 2);

    shared_ptr<Pos> pos = make_shared<Pos>(0, 0);
    shared_ptr<Edid> edid;
    list<std::shared_ptr<Mode>> modes = {mode1, mode2};
};

TEST_F(Output_test, validActive) {
    Output("validActive", Output::active, modes, mode1, nullptr, pos, edid);
}

TEST_F(Output_test, validConnected) {
    Output("validConnected", Output::connected, modes, nullptr, nullptr, nullptr, edid);
}

TEST_F(Output_test, validDisconnected) {
    Output("validDisconnected", Output::disconnected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr, edid);
}

TEST_F(Output_test, activeMissingCurrentMode) {
    EXPECT_THROW(Output("activeMissingCurrentMode", Output::active, modes, nullptr, nullptr, pos, edid),
                 invalid_argument);
}

TEST_F(Output_test, activeMissingCurrentPos) {
    EXPECT_THROW(Output("activeMissingCurrentPos", Output::active, modes, mode1, nullptr, nullptr, edid),
                 invalid_argument);
}

TEST_F(Output_test, activeEmptyModes) {
    EXPECT_THROW(Output("activeEmptyModes", Output::active, list<std::shared_ptr<Mode>>(), mode1, nullptr, pos, edid),
                 invalid_argument);
}

TEST_F(Output_test, connectedEmptyModes) {
    EXPECT_THROW(
            Output("connectedEmptyModes", Output::connected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr,
                  edid),
            invalid_argument);
}

TEST_F(Output_test, activePreferredNotInModes) {
    EXPECT_THROW(Output("activePreferredNotInModes", Output::active, modes, nullptr, modeInexistent, nullptr, edid),
                 invalid_argument);
}

TEST_F(Output_test, connectedPreferredNotInModes) {
    EXPECT_THROW(Output("connectedPreferredNotInModes", Output::connected, modes, nullptr, modeInexistent, nullptr, edid),
                 invalid_argument);
}

TEST_F(Output_test, disconnectedPreferredNotInModes) {
    Output("disconnectedPreferredNotInModes", Output::disconnected, modes, nullptr, modeInexistent, nullptr, edid);
}
