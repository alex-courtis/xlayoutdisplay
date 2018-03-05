#include <gtest/gtest.h>

#include "../src/Displ.h"

using namespace std;

class Displ_test : public ::testing::Test {
protected:
    std::shared_ptr<Mode> mode1 = make_shared<Mode>(0, 1, 2, 3);
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(4, 5, 6, 7);
    std::shared_ptr<Mode> modeInexistent = make_shared<Mode>(5, 4, 3, 2);

    shared_ptr<Pos> pos = make_shared<Pos>(0, 0);
    shared_ptr<Edid> edid;
    list<std::shared_ptr<Mode>> modes = {mode1, mode2};
};

TEST_F(Displ_test, validActive) {
    Displ("validActive", Displ::active, modes, mode1, nullptr, pos, edid);
}

TEST_F(Displ_test, validConnected) {
    Displ("validConnected", Displ::connected, modes, nullptr, nullptr, nullptr, edid);
}

TEST_F(Displ_test, validDisconnected) {
    Displ("validDisconnected", Displ::disconnected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr, edid);
}

TEST_F(Displ_test, activeMissingCurrentMode) {
    EXPECT_THROW(Displ("activeMissingCurrentMode", Displ::active, modes, nullptr, nullptr, pos, edid),
                 invalid_argument);
}

TEST_F(Displ_test, activeMissingCurrentPos) {
    EXPECT_THROW(Displ("activeMissingCurrentPos", Displ::active, modes, mode1, nullptr, nullptr, edid),
                 invalid_argument);
}

TEST_F(Displ_test, activeEmptyModes) {
    EXPECT_THROW(Displ("activeEmptyModes", Displ::active, list<std::shared_ptr<Mode>>(), mode1, nullptr, pos, edid),
                 invalid_argument);
}

TEST_F(Displ_test, connectedEmptyModes) {
    EXPECT_THROW(
            Displ("connectedEmptyModes", Displ::connected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr,
                  edid),
            invalid_argument);
}

TEST_F(Displ_test, activePreferredNotInModes) {
    EXPECT_THROW(Displ("activePreferredNotInModes", Displ::active, modes, nullptr, modeInexistent, nullptr, edid),
                 invalid_argument);
}

TEST_F(Displ_test, connectedPreferredNotInModes) {
    EXPECT_THROW(Displ("connectedPreferredNotInModes", Displ::connected, modes, nullptr, modeInexistent, nullptr, edid),
                 invalid_argument);
}

TEST_F(Displ_test, disconnectedPreferredNotInModes) {
    Displ("disconnectedPreferredNotInModes", Displ::disconnected, modes, nullptr, modeInexistent, nullptr, edid);
}

TEST_F(Displ_test, optimalPresent) {
    Displ displ = Displ("optimalPresent", Displ::disconnected, {modeInexistent}, nullptr, nullptr, nullptr, nullptr);
    displ.desiredActive(true);
}

TEST_F(Displ_test, missingOptimal) {
    Displ displ = Displ("missingOptimal", Displ::disconnected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr,
                        nullptr);
    EXPECT_THROW(displ.desiredActive(true), invalid_argument);
}
