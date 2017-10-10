#include <gtest/gtest.h>

#include "../src/arrangement.h"

using namespace std;

TEST(arrangement_orderDispls, reposition) {

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


class arrangement_activateDispls : public ::testing::Test {
protected:
    virtual void SetUp() {
        Displ::desiredPrimary.reset();
        Displ::desiredDpi = DEFAULT_DPI;
    }

    std::shared_ptr<Mode> mode = make_shared<Mode>(0, 0, 0, 0);
    shared_ptr<Pos> pos = make_shared<Pos>(0, 0);
    list<std::shared_ptr<Mode>> modes = {mode};
};

TEST_F(arrangement_activateDispls, primarySpecifiedAndLaptop) {
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

TEST_F(arrangement_activateDispls, defaultPrimary) {

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


TEST(arrangement_ltrDispls, arrange) {

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


TEST(arrangement_mirrorDisplays, noneActive) {

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

TEST(arrangement_mirrorDisplays, oneActive) {

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

TEST(arrangement_mirrorDisplays, someActive) {

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

TEST(arrangement_mirrorDisplays, manyActive) {

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

TEST(arrangement_mirrorDisplays, noCommon) {

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
