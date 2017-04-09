#include <gtest/gtest.h>

#include "../src/layout.h"

using namespace std;

TEST(OrderDispls, Reposition) {

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

    orderDispls(displs, { "FOUR", "THREE", "TWO" });

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

TEST(LtrDisplays, Arrange) {

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
