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


TEST(layout_orderDispls, reposition) {

    list<shared_ptr<Displ>> displs;
    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ1);
    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ2);
    shared_ptr<Displ> displ3 = make_shared<Displ>("Three", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ3);
    shared_ptr<Displ> displ4 = make_shared<Displ>("Four", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ4);
    shared_ptr<Displ> displ5 = make_shared<Displ>("Five", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ5);

    orderDispls(displs, {"FOUR", "THREE", "TWO"});

    EXPECT_EQ(displ4, displs.front());
    displs.pop_front();
    EXPECT_EQ(displ3, displs.front());
    displs.pop_front();
    EXPECT_EQ(displ2, displs.front());
    displs.pop_front();
    EXPECT_EQ(displ1, displs.front());
    displs.pop_front();
    EXPECT_EQ(displ5, displs.front());
}


class layout_activateDispls : public ::testing::Test {
protected:
    virtual void SetUp() {
        Displ::desiredPrimary.reset();
        Displ::desiredDpi = DEFAULT_DPI;
    }

    std::shared_ptr<Mode> mode = make_shared<Mode>(0, 0, 0, 0);
    shared_ptr<Pos> pos = make_shared<Pos>(0, 0);
    list<std::shared_ptr<Mode>> modes = {mode};
};

TEST_F(layout_activateDispls, primarySpecifiedAndLaptop) {
    list<shared_ptr<Displ>> displs;

    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::active, modes, mode, mode, pos, shared_ptr<Edid>());
    displs.push_back(displ1);

    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected, modes, mode, mode, pos,
                                                  shared_ptr<Edid>());
    displs.push_back(displ2);

    shared_ptr<Displ> displ3 = make_shared<Displ>("Three", Displ::connected, modes, mode, mode, pos,
                                                  shared_ptr<Edid>());
    displs.push_back(displ3);

    shared_ptr<Displ> displ4 = make_shared<Displ>(LAPTOP_DISPLAY_PREFIX + string("Four"), Displ::active, modes, mode,
                                                  mode, pos, shared_ptr<Edid>());
    displs.push_back(displ4);

    activateDispls(displs, "three", Monitors(true));

    EXPECT_TRUE(displ1->desiredActive());
    EXPECT_FALSE(displ2->desiredActive());
    EXPECT_TRUE(displ3->desiredActive());
    EXPECT_FALSE(displ4->desiredActive());

    EXPECT_EQ(Displ::desiredPrimary, displ3);
}

TEST_F(layout_activateDispls, defaultPrimary) {

    list<shared_ptr<Displ>> displs;

    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::disconnected, modes, mode, mode, pos,
                                                  shared_ptr<Edid>());
    displs.push_back(displ1);

    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::active, modes, mode, mode, pos, shared_ptr<Edid>());
    displs.push_back(displ2);

    shared_ptr<Displ> displ3 = make_shared<Displ>("Three", Displ::active, modes, mode, mode, pos, shared_ptr<Edid>());
    displs.push_back(displ3);

    activateDispls(displs, "noprimary", Monitors(true));

    EXPECT_FALSE(displ1->desiredActive());
    EXPECT_TRUE(displ2->desiredActive());
    EXPECT_TRUE(displ3->desiredActive());

    EXPECT_EQ(Displ::desiredPrimary, displ2);
}


TEST(layout_ltrDispls, arrange) {

    list<shared_ptr<Displ>> displs;
    list<std::shared_ptr<Mode>> modes;

    modes = {make_shared<Mode>(0, 10, 20, 30)};
    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::connected, modes, std::shared_ptr<Mode>(),
                                                  modes.front(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displ1->desiredActive(true);
    displs.push_back(displ1);

    modes = {};
    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected, modes, std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ2);

    modes = {make_shared<Mode>(0, 50, 60, 70)};
    shared_ptr<Displ> displ3 = make_shared<Displ>("Three", Displ::connected, modes, std::shared_ptr<Mode>(),
                                                  modes.front(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displ3->desiredActive(true);
    displs.push_back(displ3);

    ltrDispls(displs);

    EXPECT_TRUE(displ1->desiredActive());
    EXPECT_TRUE(displ1->desiredMode());
    EXPECT_EQ(10, displ1->desiredMode()->width);
    EXPECT_EQ(20, displ1->desiredMode()->height);
    EXPECT_EQ(30, displ1->desiredMode()->refresh);
    EXPECT_TRUE(displ1->desiredPos);
    EXPECT_EQ(0, displ1->desiredPos->x);
    EXPECT_EQ(0, displ1->desiredPos->y);

    EXPECT_FALSE(displ2->desiredActive());
    EXPECT_FALSE(displ2->desiredMode());
    EXPECT_FALSE(displ2->desiredPos);

    EXPECT_TRUE(displ3->desiredActive());
    EXPECT_TRUE(displ3->desiredMode());
    EXPECT_EQ(50, displ3->desiredMode()->width);
    EXPECT_EQ(60, displ3->desiredMode()->height);
    EXPECT_EQ(70, displ3->desiredMode()->refresh);
    EXPECT_TRUE(displ3->desiredPos);
    EXPECT_EQ(10, displ3->desiredPos->x);
    EXPECT_EQ(0, displ3->desiredPos->y);
}


TEST(layout_mirrorDisplays, noneActive) {

    list<shared_ptr<Displ>> displs;

    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ1);

    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ2);

    mirrorDispls(displs);

    EXPECT_FALSE(displ1->desiredMode());
    EXPECT_FALSE(displ1->desiredPos);

    EXPECT_FALSE(displ2->desiredMode());
    EXPECT_FALSE(displ2->desiredPos);
}

TEST(layout_mirrorDisplays, oneActive) {

    list<shared_ptr<Displ>> displs;

    const std::shared_ptr<Mode> mode2 = make_shared<Mode>(0, 5, 6, 0);
    const std::shared_ptr<Mode> mode1 = make_shared<Mode>(0, 7, 8, 0);

    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode1, mode2}), std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    displ1->desiredActive(true);
    displs.push_back(displ1);

    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ2);

    mirrorDispls(displs);

    EXPECT_EQ(mode1, displ1->desiredMode());
    EXPECT_EQ(0, displ1->desiredPos->x);
    EXPECT_EQ(0, displ1->desiredPos->y);

    EXPECT_FALSE(displ2->desiredMode());
    EXPECT_FALSE(displ2->desiredPos);
}

TEST(layout_mirrorDisplays, someActive) {

    list<shared_ptr<Displ>> displs;

    std::shared_ptr<Mode> mode5 = make_shared<Mode>(0, 1, 2, 0);
    std::shared_ptr<Mode> mode4 = make_shared<Mode>(0, 1, 2, 1);
    std::shared_ptr<Mode> mode3 = make_shared<Mode>(0, 3, 4, 0);
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(0, 5, 6, 0);
    std::shared_ptr<Mode> mode1 = make_shared<Mode>(0, 7, 8, 0);

    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode1, mode2}), std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    displs.push_back(displ1);

    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode3, mode5, mode4}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displ2->desiredActive(true);
    displs.push_back(displ2);

    shared_ptr<Displ> displ3 = make_shared<Displ>("Three", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode2, mode5, mode4}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displ3->desiredActive(true);
    displs.push_back(displ3);

    mirrorDispls(displs);

    EXPECT_FALSE(displ1->desiredMode());
    EXPECT_FALSE(displ1->desiredPos);

    EXPECT_EQ(mode4, displ2->desiredMode());
    EXPECT_EQ(0, displ2->desiredPos->x);
    EXPECT_EQ(0, displ2->desiredPos->y);

    EXPECT_EQ(mode4, displ3->desiredMode());
    EXPECT_EQ(0, displ3->desiredPos->x);
    EXPECT_EQ(0, displ3->desiredPos->y);
}

TEST(layout_mirrorDisplays, manyActive) {

    list<shared_ptr<Displ>> displs;

    std::shared_ptr<Mode> mode4 = make_shared<Mode>(0, 1, 2, 0);
    std::shared_ptr<Mode> mode3 = make_shared<Mode>(0, 3, 4, 0);
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(0, 5, 6, 0);
    std::shared_ptr<Mode> mode1 = make_shared<Mode>(0, 7, 8, 0);

    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode1, mode2, mode3}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displ1->desiredActive(true);
    displs.push_back(displ1);

    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode3, mode4}), std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    displ2->desiredActive(true);
    displs.push_back(displ2);

    shared_ptr<Displ> displ3 = make_shared<Displ>("Three", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode2, mode3}), std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    displ3->desiredActive(true);
    displs.push_back(displ3);

    mirrorDispls(displs);

    EXPECT_EQ(mode3, displ1->desiredMode());
    EXPECT_EQ(0, displ1->desiredPos->x);
    EXPECT_EQ(0, displ1->desiredPos->y);

    EXPECT_EQ(mode3, displ2->desiredMode());
    EXPECT_EQ(0, displ2->desiredPos->x);
    EXPECT_EQ(0, displ2->desiredPos->y);

    EXPECT_EQ(mode3, displ3->desiredMode());
    EXPECT_EQ(0, displ3->desiredPos->x);
    EXPECT_EQ(0, displ3->desiredPos->y);
}

TEST(layout_mirrorDisplays, noCommon) {

    list<shared_ptr<Displ>> displs;

    std::shared_ptr<Mode> mode4 = make_shared<Mode>(0, 1, 2, 0);
    std::shared_ptr<Mode> mode3 = make_shared<Mode>(0, 3, 4, 0);
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(0, 5, 6, 0);
    std::shared_ptr<Mode> mode1 = make_shared<Mode>(0, 7, 8, 0);

    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode1, mode2}), std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    displ1->desiredActive(true);
    displs.push_back(displ1);

    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode3, mode4}), std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    displ2->desiredActive(true);
    displs.push_back(displ2);

    shared_ptr<Displ> displ3 = make_shared<Displ>("Three", Displ::disconnected,
                                                  list<std::shared_ptr<Mode>>({mode1, mode4}), std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    displ3->desiredActive(true);
    displs.push_back(displ3);

    EXPECT_THROW(mirrorDispls(displs), runtime_error);
}
