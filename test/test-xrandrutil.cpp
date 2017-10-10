#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/xrandrrutil.h"

using namespace std;
using ::testing::Return;

class MockEdid : public Edid {
public:
    MOCK_CONST_METHOD0(maxCmHoriz, const int());

    MOCK_CONST_METHOD0(maxCmVert, const int());

    MOCK_CONST_METHOD1(dpiForMode, const long(
            const std::shared_ptr<Mode> &mode));
};

TEST(xrandrutil_renderCmd, renderAll) {
    list<shared_ptr<Displ>> displs;
    list<std::shared_ptr<Mode>> modes = {make_shared<Mode>(0, 0, 0, 0)};

    shared_ptr<Displ> displ1 = make_shared<Displ>("One", Displ::disconnected, modes, std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displs.push_back(displ1);

    shared_ptr<MockEdid> edid2 = make_shared<MockEdid>();
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(0, 1, 2, 3);
    shared_ptr<Displ> displ2 = make_shared<Displ>("Two", Displ::disconnected, list<std::shared_ptr<Mode>>({mode2}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(),
                                                  shared_ptr<Pos>(), edid2);
    displ2->desiredActive(true);
    displ2->desiredMode(mode2);
    displ2->desiredPos = make_shared<Pos>(5, 6);
    displs.push_back(displ2);

    shared_ptr<Displ> displ3 = make_shared<Displ>("Three", Displ::disconnected, modes, std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    displ3->desiredActive(true);
    displ3->desiredPos = make_shared<Pos>(13, 14);
    displs.push_back(displ3);

    std::shared_ptr<Mode> mode4 = make_shared<Mode>(15, 16, 17, 18);
    shared_ptr<Displ> displ4 = make_shared<Displ>("Four", Displ::disconnected, list<std::shared_ptr<Mode>>({mode4}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(),
                                                  shared_ptr<Pos>(), shared_ptr<Edid>());
    displ4->desiredActive(true);
    displ4->desiredMode(mode4);
    displs.push_back(displ4);

    std::shared_ptr<Mode> mode5 = make_shared<Mode>(7, 8, 9, 10);
    shared_ptr<Displ> displ5 = make_shared<Displ>("Five", Displ::disconnected, list<std::shared_ptr<Mode>>({mode5}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(),
                                                  shared_ptr<Pos>(), shared_ptr<Edid>());
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

    EXPECT_EQ(expected.str(), renderXrandrCmd(displs));
}

TEST(xrandrutil_renderUserInfo, renderAll) {
    std::shared_ptr<Mode> mode1 = make_shared<Mode>(1, 2, 3, 4);
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(5, 6, 7, 8);
    std::shared_ptr<Mode> mode3 = make_shared<Mode>(9, 10, 11, 12);
    shared_ptr<Pos> pos = make_shared<Pos>(13, 14);
    shared_ptr<MockEdid> edid1 = make_shared<MockEdid>();
    EXPECT_CALL(*edid1, maxCmHoriz()).WillOnce(Return(15));
    EXPECT_CALL(*edid1, maxCmVert()).WillOnce(Return(16));
    shared_ptr<MockEdid> edid3 = make_shared<MockEdid>();
    EXPECT_CALL(*edid3, maxCmHoriz()).WillOnce(Return(17));
    EXPECT_CALL(*edid3, maxCmVert()).WillOnce(Return(18));

    list<shared_ptr<Displ>> displs;

    shared_ptr<Displ> dis = make_shared<Displ>("dis", Displ::disconnected, list<std::shared_ptr<Mode>>(),
                                               std::shared_ptr<Mode>(), std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                               edid1);
    displs.push_back(dis);

    shared_ptr<Displ> con = make_shared<Displ>("con", Displ::connected, list<std::shared_ptr<Mode>>({mode1, mode2}),
                                               std::shared_ptr<Mode>(), std::shared_ptr<Mode>(),
                                               shared_ptr<Pos>(), shared_ptr<Edid>());
    displs.push_back(con);

    shared_ptr<Displ> act = make_shared<Displ>("act", Displ::active, list<std::shared_ptr<Mode>>({mode3, mode2, mode1}),
                                               mode2, mode3,
                                               pos, edid3);
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

    EXPECT_CALL(xrrWrapper, xOpenDisplay(nullptr));

    EXPECT_THROW(discoverDispls(&xrrWrapper), runtime_error);
}

TEST_F(xrandrutil_discoverDispls, excessScreens) {
    MockXrrWrapper xrrWrapper;

    EXPECT_CALL(xrrWrapper, xOpenDisplay(nullptr)).WillOnce(Return(dpy));
    EXPECT_CALL(xrrWrapper, defaultScreen(dpy)).WillOnce(Return(screen));
    EXPECT_CALL(xrrWrapper, screenCount(dpy)).Times(2).WillRepeatedly(Return(1));

    EXPECT_THROW(discoverDispls(&xrrWrapper), runtime_error);
}

TEST_F(xrandrutil_discoverDispls, noDisplays) {
    MockXrrWrapper xrrWrapper;

    EXPECT_CALL(xrrWrapper, xOpenDisplay(nullptr)).WillOnce(Return(dpy));
    EXPECT_CALL(xrrWrapper, defaultScreen(dpy)).WillOnce(Return(screen));
    EXPECT_CALL(xrrWrapper, screenCount(dpy)).WillOnce(Return(3));
    EXPECT_CALL(xrrWrapper, rootWindow(dpy, screen)).WillOnce(Return(rootWindow));
    EXPECT_CALL(xrrWrapper, xrrGetScreenResources(dpy, rootWindow)).WillOnce(Return(&screenResources));

    screenResources.noutput = 0;

    const list<shared_ptr<Displ>> displs = discoverDispls(&xrrWrapper);

    EXPECT_TRUE(displs.empty());
}

class xrandrutil_modeFromXRR : public ::testing::Test {
protected:
    virtual void SetUp() {
        resources.nmode = 3;
        modeInfos[0].id = 10;
        modeInfos[1].id = 11;
        modeInfos[2].id = 12;
        resources.modes = &modeInfos[0];
    }

    XRRScreenResources resources;
    XRRModeInfo modeInfos[3];
};

TEST_F(xrandrutil_modeFromXRR, valid) {
    modeFromXRR(11, &resources);
}

TEST_F(xrandrutil_modeFromXRR, modeNotPresent) {
    EXPECT_THROW(modeFromXRR(13, &resources), invalid_argument);
}

TEST_F(xrandrutil_modeFromXRR, resourcesNotPresent) {
    EXPECT_THROW(modeFromXRR(11, nullptr), invalid_argument);
}

// TODO: fully test discoverDispls