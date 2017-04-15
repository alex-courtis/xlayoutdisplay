#include <gtest/gtest.h>

#include "../src/Laptop.h"
#include "../src/layout.h"

using namespace std;

TEST(layout_orderDispls, reposition) {

    list <DisplP> displs;
    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displs.push_back(displ1);
    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displs.push_back(displ2);
    DisplP displ3 = make_shared<Displ>("Three", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displs.push_back(displ3);
    DisplP displ4 = make_shared<Displ>("Four", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displs.push_back(displ4);
    DisplP displ5 = make_shared<Displ>("Five", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
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
        Displ::desiredPrimary = DisplP();
    }

    void TearDown() override {
        Laptop::singletonInstance = NULL;
    }

    void setLidClosed(const bool closed) {
        Laptop::instance()->lidClosed = closed;
    }

    ModeP mode = make_shared<Mode>(0, 0, 0, 0);
    PosP pos = make_shared<Pos>(0, 0);
    list <ModeP> modes = {mode};
};

TEST_F(layout_activateDispls, primarySpecifiedAndLaptop) {
    list <DisplP> displs;

    DisplP displ1 = make_shared<Displ>("One", Displ::active, modes, mode, mode, mode, pos);
    displs.push_back(displ1);

    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, modes, mode, mode, mode, pos);
    displs.push_back(displ2);

    DisplP displ3 = make_shared<Displ>("Three", Displ::connected, modes, mode, mode, mode, pos);
    displs.push_back(displ3);

    DisplP displ4 = make_shared<Displ>(Laptop::embeddedDisplayPrefix() + string("Four"), Displ::active, modes, mode, mode, mode, pos);
    displs.push_back(displ4);

    setLidClosed(true);

    activateDispls(displs, "three");

    EXPECT_TRUE(displ1->desiredActive);
    EXPECT_FALSE(displ2->desiredActive);
    EXPECT_TRUE(displ3->desiredActive);
    EXPECT_FALSE(displ4->desiredActive);

    EXPECT_EQ(Displ::desiredPrimary, displ3);
}

TEST_F(layout_activateDispls, defaultPrimary) {

    list <DisplP> displs;

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, modes, mode, mode, mode, pos);
    displs.push_back(displ1);

    DisplP displ2 = make_shared<Displ>("Two", Displ::active, modes, mode, mode, mode, pos);
    displs.push_back(displ2);

    DisplP displ3 = make_shared<Displ>("Three", Displ::active, modes, mode, mode, mode, pos);
    displs.push_back(displ3);

    activateDispls(displs, "noprimary");

    setLidClosed(false);

    EXPECT_FALSE(displ1->desiredActive);
    EXPECT_TRUE(displ2->desiredActive);
    EXPECT_TRUE(displ3->desiredActive);

    EXPECT_EQ(Displ::desiredPrimary, displ2);
}

TEST(layout_ltrDisplays, arrange) {

    list <DisplP> displs;

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), make_shared<Mode>(0, 10, 20, 0), PosP());
    displ1->desiredActive = true;
    displs.push_back(displ1);

    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), make_shared<Mode>(0, 30, 40, 0), PosP());
    displ2->desiredActive = false;
    displs.push_back(displ2);

    DisplP displ3 = make_shared<Displ>("Three", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), make_shared<Mode>(0, 50, 60, 0), PosP());
    displ3->desiredActive = true;
    displs.push_back(displ3);

    ltrDispls(displs);

    EXPECT_TRUE(displ1->desiredActive);
    EXPECT_TRUE(displ1->desiredMode);
    EXPECT_EQ(displ1->optimalMode, displ1->desiredMode);
    EXPECT_TRUE(displ1->desiredPos);
    EXPECT_EQ(0, displ1->desiredPos->x);
    EXPECT_EQ(0, displ1->desiredPos->y);

    EXPECT_FALSE(displ2->desiredActive);
    EXPECT_FALSE(displ2->desiredMode);
    EXPECT_FALSE(displ2->desiredPos);

    EXPECT_TRUE(displ3->desiredActive);
    EXPECT_TRUE(displ3->desiredMode);
    EXPECT_EQ(displ3->optimalMode, displ3->desiredMode);
    EXPECT_TRUE(displ3->desiredPos);
    EXPECT_EQ(10, displ3->desiredPos->x);
    EXPECT_EQ(0, displ3->desiredPos->y);
}
