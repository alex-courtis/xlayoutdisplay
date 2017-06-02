#include <gtest/gtest.h>

#include "../src/Laptop.h"
#include "../src/Layout.h"

using namespace std;

class abstractLayoutTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        Displ::desiredPrimary.reset();
        Displ::desiredDpi = DEFAULT_DPI;
        Laptop::singletonInstance = NULL;
    }

    void setLidClosed(const bool closed) {
        Laptop::instance()->lidClosed = closed;
    }
};


TEST(Layout_orderDispls, reposition) {

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), EdidP());
    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), EdidP());
    DisplP displ3 = make_shared<Displ>("Three", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), EdidP());
    DisplP displ4 = make_shared<Displ>("Four", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), EdidP());
    DisplP displ5 = make_shared<Displ>("Five", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), EdidP());

    Layout layout = Layout({displ1, displ2, displ3, displ4, displ5});

    layout.orderDispls({"FOUR", "THREE", "TWO"});

    EXPECT_EQ(displ4, layout.displs.front());
    layout.displs.pop_front();
    EXPECT_EQ(displ3, layout.displs.front());
    layout.displs.pop_front();
    EXPECT_EQ(displ2, layout.displs.front());
    layout.displs.pop_front();
    EXPECT_EQ(displ1, layout.displs.front());
    layout.displs.pop_front();
    EXPECT_EQ(displ5, layout.displs.front());
}


class Layout_activateDispls : public abstractLayoutTest {

protected:
    ModeP mode = make_shared<Mode>(0, 0, 0, 0);
    PosP pos = make_shared<Pos>(0, 0);
    list<ModeP> modes = {mode};
};

TEST_F(Layout_activateDispls, primarySpecifiedAndLaptop) {

    DisplP displ1 = make_shared<Displ>("One", Displ::active, modes, mode, mode, pos, EdidP());
    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, modes, mode, mode, pos, EdidP());
    DisplP displ3 = make_shared<Displ>("Three", Displ::connected, modes, mode, mode, pos, EdidP());
    DisplP displ4 = make_shared<Displ>(embeddedDisplayPrefix() + string("Four"), Displ::active, modes, mode, mode, pos, EdidP());
    Layout layout = Layout({displ1, displ2, displ3, displ4});

    setLidClosed(true);

    layout.activateDispls("three");

    EXPECT_TRUE(displ1->desiredActive());
    EXPECT_FALSE(displ2->desiredActive());
    EXPECT_TRUE(displ3->desiredActive());
    EXPECT_FALSE(displ4->desiredActive());

    EXPECT_EQ(Displ::desiredPrimary, displ3);
}

TEST_F(Layout_activateDispls, defaultPrimary) {

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, modes, mode, mode, pos, EdidP());
    DisplP displ2 = make_shared<Displ>("Two", Displ::active, modes, mode, mode, pos, EdidP());
    DisplP displ3 = make_shared<Displ>("Three", Displ::active, modes, mode, mode, pos, EdidP());
    Layout layout = Layout({displ1, displ2, displ3});

    layout.activateDispls("noprimary");

    setLidClosed(false);

    EXPECT_FALSE(displ1->desiredActive());
    EXPECT_TRUE(displ2->desiredActive());
    EXPECT_TRUE(displ3->desiredActive());

    EXPECT_EQ(Displ::desiredPrimary, displ2);
}


class Layout_ltrDispls : public abstractLayoutTest {
};

TEST(Layout_ltrDispls, arrange) {

    list<ModeP> modes;

    modes = {make_shared<Mode>(0, 10, 20, 30)};
    DisplP displ1 = make_shared<Displ>("One", Displ::connected, modes, ModeP(), modes.front(), PosP(), EdidP());
    displ1->desiredActive(true);

    modes = {};
    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, modes, ModeP(), ModeP(), PosP(), EdidP());

    modes = {make_shared<Mode>(0, 50, 60, 70)};
    DisplP displ3 = make_shared<Displ>("Three", Displ::connected, modes, ModeP(), modes.front(), PosP(), EdidP());
    displ3->desiredActive(true);

    Layout layout = Layout({displ1, displ2, displ3});

    layout.ltrDispls();

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


class Layout_mirrorDisplays : public abstractLayoutTest {
};

TEST(Layout_mirrorDisplays, noneActive) {

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), EdidP());
    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), EdidP());

    Layout layout = Layout({displ1, displ2});

    layout.mirrorDispls();

    EXPECT_FALSE(displ1->desiredMode());
    EXPECT_FALSE(displ1->desiredPos);

    EXPECT_FALSE(displ2->desiredMode());
    EXPECT_FALSE(displ2->desiredPos);
}

TEST(Layout_mirrorDisplays, oneActive) {

    const ModeP mode2 = make_shared<Mode>(0, 5, 6, 0);
    const ModeP mode1 = make_shared<Mode>(0, 7, 8, 0);

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>({mode1, mode2}), ModeP(), ModeP(), PosP(), EdidP());
    displ1->desiredActive(true);
    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), EdidP());

    Layout layout = Layout({displ1, displ2});

    layout.mirrorDispls();

    EXPECT_EQ(mode1, displ1->desiredMode());
    EXPECT_EQ(0, displ1->desiredPos->x);
    EXPECT_EQ(0, displ1->desiredPos->y);

    EXPECT_FALSE(displ2->desiredMode());
    EXPECT_FALSE(displ2->desiredPos);
}

TEST(Layout_mirrorDisplays, someActive) {

    ModeP mode5 = make_shared<Mode>(0, 1, 2, 0);
    ModeP mode4 = make_shared<Mode>(0, 1, 2, 1);
    ModeP mode3 = make_shared<Mode>(0, 3, 4, 0);
    ModeP mode2 = make_shared<Mode>(0, 5, 6, 0);
    ModeP mode1 = make_shared<Mode>(0, 7, 8, 0);

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>({mode1, mode2}), ModeP(), ModeP(), PosP(), EdidP());

    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>({mode3, mode5, mode4}), ModeP(), ModeP(), PosP(), EdidP());
    displ2->desiredActive(true);

    DisplP displ3 = make_shared<Displ>("Three", Displ::disconnected, list<ModeP>({mode2, mode5, mode4}), ModeP(), ModeP(), PosP(), EdidP());
    displ3->desiredActive(true);

    Layout layout = Layout({displ1, displ2, displ3});

    layout.mirrorDispls();

    EXPECT_FALSE(displ1->desiredMode());
    EXPECT_FALSE(displ1->desiredPos);

    EXPECT_EQ(mode4, displ2->desiredMode());
    EXPECT_EQ(0, displ2->desiredPos->x);
    EXPECT_EQ(0, displ2->desiredPos->y);

    EXPECT_EQ(mode4, displ3->desiredMode());
    EXPECT_EQ(0, displ3->desiredPos->x);
    EXPECT_EQ(0, displ3->desiredPos->y);
}

TEST(Layout_mirrorDisplays, manyActive) {

    ModeP mode4 = make_shared<Mode>(0, 1, 2, 0);
    ModeP mode3 = make_shared<Mode>(0, 3, 4, 0);
    ModeP mode2 = make_shared<Mode>(0, 5, 6, 0);
    ModeP mode1 = make_shared<Mode>(0, 7, 8, 0);

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>({mode1, mode2, mode3}), ModeP(), ModeP(), PosP(), EdidP());
    displ1->desiredActive(true);

    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>({mode3, mode4}), ModeP(), ModeP(), PosP(), EdidP());
    displ2->desiredActive(true);

    DisplP displ3 = make_shared<Displ>("Three", Displ::disconnected, list<ModeP>({mode2, mode3}), ModeP(), ModeP(), PosP(), EdidP());
    displ3->desiredActive(true);

    Layout layout = Layout({displ1, displ2, displ3});

    layout.mirrorDispls();

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

TEST(Layout_mirrorDisplays, noCommon) {

    ModeP mode4 = make_shared<Mode>(0, 1, 2, 0);
    ModeP mode3 = make_shared<Mode>(0, 3, 4, 0);
    ModeP mode2 = make_shared<Mode>(0, 5, 6, 0);
    ModeP mode1 = make_shared<Mode>(0, 7, 8, 0);

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>({mode1, mode2}), ModeP(), ModeP(), PosP(), EdidP());
    displ1->desiredActive(true);

    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>({mode3, mode4}), ModeP(), ModeP(), PosP(), EdidP());
    displ2->desiredActive(true);

    DisplP displ3 = make_shared<Displ>("Three", Displ::disconnected, list<ModeP>({mode1, mode4}), ModeP(), ModeP(), PosP(), EdidP());
    displ3->desiredActive(true);

    Layout layout = Layout({displ1, displ2, displ3});

    EXPECT_THROW(layout.mirrorDispls(), runtime_error);
}
