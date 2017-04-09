#include <gtest/gtest.h>

#include "../src/layout.h"

using namespace std;

TEST(OrderDispls, Reposition) {
    list <ModeP> modes;
    ModeP currentMode;
    ModeP preferredMode;
    ModeP optimalMode;
    PosP currentPos;

    list <DisplP> displs;
    DisplP displ1 = make_shared<Displ>("1", Displ::disconnected, modes, currentMode, preferredMode, optimalMode, currentPos);
    displs.push_back(displ1);
    DisplP displ2 = make_shared<Displ>("2", Displ::disconnected, modes, currentMode, preferredMode, optimalMode, currentPos);
    displs.push_back(displ2);
    DisplP displ3 = make_shared<Displ>("3", Displ::disconnected, modes, currentMode, preferredMode, optimalMode, currentPos);
    displs.push_back(displ3);
    DisplP displ4 = make_shared<Displ>("4", Displ::disconnected, modes, currentMode, preferredMode, optimalMode, currentPos);
    displs.push_back(displ4);
    DisplP displ5 = make_shared<Displ>("5", Displ::disconnected, modes, currentMode, preferredMode, optimalMode, currentPos);
    displs.push_back(displ5);

    orderDispls(displs, { "4", "3", "2" });

    ASSERT_EQ(displ4, displs.front());
    displs.pop_front();
    ASSERT_EQ(displ3, displs.front());
    displs.pop_front();
    ASSERT_EQ(displ2, displs.front());
    displs.pop_front();
    ASSERT_EQ(displ1, displs.front());
    displs.pop_front();
    ASSERT_EQ(displ5, displs.front());
    displs.pop_front();
}
