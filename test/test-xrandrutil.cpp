#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/xrandrrutil.h"

using namespace std;
using ::testing::Return;

class MockEdid : public Edid {
public:
    MOCK_CONST_METHOD0(maxCmHoriz, const int());

    MOCK_CONST_METHOD0(maxCmVert, const int());

    MOCK_CONST_METHOD1(dpiForMode, const double(
            const ModeP &mode));

    MOCK_CONST_METHOD1(closestDpiForMode, const unsigned int(
            const ModeP &mode));
};

TEST(xrandrutil_renderCmd, renderAll) {
    list <DisplP> displs;
    list <ModeP> modes = {make_shared<Mode>(0, 0, 0, 0)};

    DisplP displ1 = make_shared<Displ>("One", Displ::disconnected, modes, ModeP(), ModeP(), PosP(), EdidP());
    displs.push_back(displ1);

    shared_ptr<MockEdid> edid2 = make_shared<MockEdid>();
    ModeP mode2 = make_shared<Mode>(0, 1, 2, 3);
    DisplP displ2 = make_shared<Displ>("Two", Displ::disconnected, list<ModeP>({mode2}), ModeP(), ModeP(), PosP(), edid2);
    displ2->desiredActive(true);
    displ2->desiredMode(mode2);
    displ2->desiredPos = make_shared<Pos>(5, 6);
    displs.push_back(displ2);

    DisplP displ3 = make_shared<Displ>("Three", Displ::disconnected, modes, ModeP(), ModeP(), PosP(), EdidP());
    displ3->desiredActive(true);
    displ3->desiredPos = make_shared<Pos>(13, 14);
    displs.push_back(displ3);

    ModeP mode4 = make_shared<Mode>(15, 16, 17, 18);
    DisplP displ4 = make_shared<Displ>("Four", Displ::disconnected, list<ModeP>({mode4}), ModeP(), ModeP(), PosP(), EdidP());
    displ4->desiredActive(true);
    displ4->desiredMode(mode4);
    displs.push_back(displ4);

    ModeP mode5 = make_shared<Mode>(7, 8, 9, 10);
    DisplP displ5 = make_shared<Displ>("Five", Displ::disconnected, list<ModeP>({mode5}), ModeP(), ModeP(), PosP(), EdidP());
    displ5->desiredActive(true);
    displ5->desiredMode(mode5);
    displ5->desiredPos = make_shared<Pos>(11, 12);
    displs.push_back(displ5);

    Displ::desiredPrimary = displ2;

    stringstream expected;
    expected << "xrandr \\\n";
    expected << " --dpi " << DEFAULT_DPI << " \\\n";
    expected << " --output One --off \\\n";
    expected << " --output Two --mode 1x2 --rate 3 --pos 5x6 --primary \\\n";
    expected << " --output Three --off \\\n";
    expected << " --output Four --off \\\n";
    expected << " --output Five --mode 8x9 --rate 10 --pos 11x12";

    EXPECT_EQ(expected.str(), renderCmd(displs));
}

TEST(xrandrutil_renderUserInfo, renderAll) {
    ModeP mode1 = make_shared<Mode>(1, 2, 3, 4);
    ModeP mode2 = make_shared<Mode>(5, 6, 7, 8);
    ModeP mode3 = make_shared<Mode>(9, 10, 11, 12);
    PosP pos = make_shared<Pos>(13, 14);
    shared_ptr<MockEdid> edid1 = make_shared<MockEdid>();
    EXPECT_CALL(*edid1, maxCmHoriz()).WillOnce(Return(15));
    EXPECT_CALL(*edid1, maxCmVert()).WillOnce(Return(16));
    shared_ptr<MockEdid> edid3 = make_shared<MockEdid>();
    EXPECT_CALL(*edid3, maxCmHoriz()).WillOnce(Return(17));
    EXPECT_CALL(*edid3, maxCmVert()).WillOnce(Return(18));

    list <DisplP> displs;

    DisplP dis = make_shared<Displ>("dis", Displ::disconnected, list<ModeP>(), ModeP(), ModeP(), PosP(), edid1);
    displs.push_back(dis);

    DisplP con = make_shared<Displ>("con", Displ::connected, list<ModeP>({mode1, mode2}), ModeP(), ModeP(), PosP(), EdidP());
    displs.push_back(con);

    DisplP act = make_shared<Displ>("act", Displ::active, list<ModeP>({mode3, mode2, mode1}), mode2, mode3, pos, edid3);
    displs.push_back(act);

    const string expected = ""
            "dis disconnected 15cm/16cm\n"
            "con connected\n"
            "  !6x7 8Hz\n"
            "   2x3 4Hz\n"
            "act active 17cm/18cm 6x7+13+14 8Hz\n"
            " +!10x11 12Hz\n"
            "*  6x7 8Hz\n"
            "   2x3 4Hz\n"
            "*current +preferred !optimal";

    EXPECT_EQ(expected, renderUserInfo(displs));

}

class MockXrrWrapper : public XrrWrapper {
public:
    MOCK_CONST_METHOD1(xOpenDisplay, Display *(_Xconst
            char*));

    MOCK_CONST_METHOD1(defaultScreen, int(Display
            *));

    MOCK_CONST_METHOD1(screenCount, int(Display
            *));

    MOCK_CONST_METHOD2(rootWindow, Window(Display * , int));

    MOCK_CONST_METHOD2(xrrGetScreenResources, XRRScreenResources *(Display
            *, Window));

    MOCK_CONST_METHOD3(xrrGetOutputInfo, XRROutputInfo *(Display
            *, XRRScreenResources *, RROutput));

    MOCK_CONST_METHOD3(xrrGetCrtcInfo, XRRCrtcInfo *(Display
            *, XRRScreenResources *, RRCrtc));
};

class xrandrutil_discoverDispls : public ::testing::Test {
protected:
    void SetUp() override {
    }

    Display *dpy = (Display *) 1;
    int screen = 2;
    Window rootWindow;
    XRRScreenResources screenResources;
};

TEST_F(xrandrutil_discoverDispls, cannotOpenDisplay) {
    MockXrrWrapper xrrWrapper;

    EXPECT_CALL(xrrWrapper, xOpenDisplay(NULL));

    EXPECT_THROW(discoverDispls(&xrrWrapper), runtime_error);
}

TEST_F(xrandrutil_discoverDispls, excessScreens) {
    MockXrrWrapper xrrWrapper;

    EXPECT_CALL(xrrWrapper, xOpenDisplay(NULL)).WillOnce(Return(dpy));
    EXPECT_CALL(xrrWrapper, defaultScreen(dpy)).WillOnce(Return(screen));
    EXPECT_CALL(xrrWrapper, screenCount(dpy)).Times(2).WillRepeatedly(Return(1));

    EXPECT_THROW(discoverDispls(&xrrWrapper), runtime_error);
}

TEST_F(xrandrutil_discoverDispls, noDisplays) {
    MockXrrWrapper xrrWrapper;

    EXPECT_CALL(xrrWrapper, xOpenDisplay(NULL)).WillOnce(Return(dpy));
    EXPECT_CALL(xrrWrapper, defaultScreen(dpy)).WillOnce(Return(screen));
    EXPECT_CALL(xrrWrapper, screenCount(dpy)).WillOnce(Return(3));
    EXPECT_CALL(xrrWrapper, rootWindow(dpy, screen)).WillOnce(Return(rootWindow));
    EXPECT_CALL(xrrWrapper, xrrGetScreenResources(dpy, rootWindow)).WillOnce(Return(&screenResources));

    screenResources.noutput = 0;

    const list <DisplP> displs = discoverDispls(&xrrWrapper);

    EXPECT_TRUE(displs.empty());
}

// todo: fully test discoverDispls