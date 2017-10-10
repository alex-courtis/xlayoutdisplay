#include <gtest/gtest.h>

#include "../src/Displ.h"

using namespace std;

class Displ_constructor : public ::testing::Test {
protected:
    std::shared_ptr<Mode> mode1 = make_shared<Mode>(0, 1, 2, 3);
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(4, 5, 6, 7);

    shared_ptr<Pos> pos = make_shared<Pos>(0, 0);
    shared_ptr<Edid> edid;
    list<std::shared_ptr<Mode>> modes = {mode1, mode2};
};

TEST_F(Displ_constructor, validActive) {
    Displ("validActive", Displ::active, modes, mode1, nullptr, pos, edid);
}

TEST_F(Displ_constructor, validConnected) {
    Displ("validConnected", Displ::connected, modes, nullptr, nullptr, nullptr, edid);
}

TEST_F(Displ_constructor, validDisconnected) {
    Displ("validDisconnected", Displ::disconnected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr, edid);
}

TEST_F(Displ_constructor, activeMissingCurrentMode) {
    EXPECT_THROW(Displ("activeMissingCurrentMode", Displ::active, modes, nullptr, nullptr, pos, edid),
                 invalid_argument);
}

TEST_F(Displ_constructor, activeMissingCurrentPos) {
    EXPECT_THROW(Displ("activeMissingCurrentPos", Displ::active, modes, mode1, nullptr, nullptr, edid),
                 invalid_argument);
}

TEST_F(Displ_constructor, activeEmptyModes) {
    EXPECT_THROW(Displ("activeEmptyModes", Displ::active, list<std::shared_ptr<Mode>>(), mode1, nullptr, pos, edid),
                 invalid_argument);
}

TEST_F(Displ_constructor, connectedEmptyModes) {
    EXPECT_THROW(
            Displ("connectedEmptyModes", Displ::connected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr,
                  edid),
            invalid_argument);
}

TEST_F(Displ_constructor, optimalMissing) {
    Displ displ = Displ("optimalMissing", Displ::disconnected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr,
                        edid);
    EXPECT_FALSE(displ.optimalMode);
}

TEST_F(Displ_constructor, optimalFirst) {
    Displ displ = Displ("optimalFirst", Displ::disconnected, modes, nullptr, nullptr, nullptr, edid);
    EXPECT_EQ(mode2, displ.optimalMode);
}

TEST_F(Displ_constructor, optimalPreferred) {
    Displ displ = Displ("optimalPreferred", Displ::disconnected, modes, nullptr, mode1, nullptr, edid);
    EXPECT_EQ(mode1, displ.optimalMode);
}

TEST_F(Displ_constructor, optimalPreferredHigherRefresh) {
    std::shared_ptr<Mode> mode3 = make_shared<Mode>(4, 5, 6, 70);
    modes.push_front(mode3);
    Displ displ = Displ("optimalPreferredHigherRefresh", Displ::disconnected, modes, nullptr, mode2, nullptr, edid);
    EXPECT_EQ(mode3, displ.optimalMode);
}

TEST_F(Displ_constructor, currentNotInModes) {
    EXPECT_THROW(
            Displ("currentNotInModes", Displ::connected, modes, make_shared<Mode>(1, 2, 3, 4), nullptr, nullptr, edid),
            invalid_argument);
}

TEST_F(Displ_constructor, preferredNotInModes) {
    EXPECT_THROW(Displ("preferredNotInModes", Displ::connected, modes, nullptr, make_shared<Mode>(5, 6, 7, 8), nullptr,
                       edid), invalid_argument);
}

TEST_F(Displ_constructor, disconnectedNotInModes) {
    Displ("disconnectedNotInModes", Displ::disconnected, list<std::shared_ptr<Mode>>(),
          make_shared<Mode>(11, 12, 13, 14),
          make_shared<Mode>(15, 16, 17, 18), nullptr, edid);
}


TEST(Displ_setDesiredActive, optimalPresent) {
    Displ displ = Displ("optimalPresent", Displ::disconnected, {make_shared<Mode>(9, 0, 9, 8)}, nullptr, nullptr,
                        nullptr, nullptr);
    displ.desiredActive(true);
}

TEST(Displ_setDesiredActive, missingOptimal) {
    Displ displ = Displ("missingOptimal", Displ::disconnected, list<std::shared_ptr<Mode>>(), nullptr, nullptr, nullptr,
                        nullptr);
    EXPECT_THROW(displ.desiredActive(true), invalid_argument);
}



