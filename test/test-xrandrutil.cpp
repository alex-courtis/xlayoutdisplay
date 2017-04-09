#include <gtest/gtest.h>

#include "../src/xrandrrutil.h"

using namespace std;

TEST(xrandrutil_render, renderAll) {
    list <DisplP> displs;

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displs.push_back(displ1);

    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displ2->desiredActive = true;
    displ2->desiredMode = make_shared<Mode>(1, 2, 3, 4);
    displ2->desiredPos = make_shared<Pos>(5, 6);
    displs.push_back(displ2);

    DisplP displ3 = make_shared<Displ>("Three", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displ3->desiredActive = true;
    displ3->desiredPos = make_shared<Pos>(13, 14);
    displs.push_back(displ3);

    DisplP displ4 = make_shared<Displ>("Four", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displ4->desiredActive = true;
    displ4->desiredMode = make_shared<Mode>(15, 16, 17, 18);
    displs.push_back(displ4);

    DisplP displ5 = make_shared<Displ>("Five", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), ModeP(), PosP());
    displ5->desiredActive = true;
    displ5->desiredMode = make_shared<Mode>(7, 8, 9, 10);
    displ5->desiredPos = make_shared<Pos>(11, 12);
    displs.push_back(displ5);

    Displ::desiredPrimary = displ2;

    const string expected = ""
            "xrandr"
            " \\\n "
            "--output One --off"
            " \\\n "
            "--output Two --mode 2x3 --rate 4 --pos 5x6 --primary"
            " \\\n "
            "--output Three --off"
            " \\\n "
            "--output Four --off"
            " \\\n "
            "--output Five --mode 8x9 --rate 10 --pos 11x12";

    EXPECT_EQ(expected, renderCmd(displs));
}