#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/xrandrrutil.h"

#include "test-MockEdid.h"

using namespace std;
using ::testing::Return;

TEST(xrandrutil_renderXrandrCmd, renderAll) {
    list<shared_ptr<Output>> outputs;
    list<std::shared_ptr<Mode>> modes = {make_shared<Mode>(0, 0, 0, 0)};

    shared_ptr<Output> output1 = make_shared<Output>("One", Output::disconnected, modes, std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    outputs.push_back(output1);

    shared_ptr<MockEdid> edid2 = make_shared<MockEdid>();
    std::shared_ptr<Mode> mode2 = make_shared<Mode>(0, 1, 2, 3);
    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected, list<std::shared_ptr<Mode>>({mode2}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(),
                                                  shared_ptr<Pos>(), edid2);
    output2->desiredActive = true;
    output2->desiredMode(mode2);
    output2->desiredPos = make_shared<Pos>(5, 6);
    outputs.push_back(output2);

    shared_ptr<Output> output3 = make_shared<Output>("Three", Output::disconnected, modes, std::shared_ptr<Mode>(),
                                                  std::shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                  shared_ptr<Edid>());
    output3->desiredActive = true;
    output3->desiredPos = make_shared<Pos>(13, 14);
    outputs.push_back(output3);

    std::shared_ptr<Mode> mode4 = make_shared<Mode>(15, 16, 17, 18);
    shared_ptr<Output> output4 = make_shared<Output>("Four", Output::disconnected, list<std::shared_ptr<Mode>>({mode4}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(),
                                                  shared_ptr<Pos>(), shared_ptr<Edid>());
    output4->desiredActive = true;
    output4->desiredMode(mode4);
    outputs.push_back(output4);

    std::shared_ptr<Mode> mode5 = make_shared<Mode>(7, 8, 9, 10);
    shared_ptr<Output> output5 = make_shared<Output>("Five", Output::disconnected, list<std::shared_ptr<Mode>>({mode5}),
                                                  std::shared_ptr<Mode>(), std::shared_ptr<Mode>(),
                                                  shared_ptr<Pos>(), shared_ptr<Edid>());
    output5->desiredActive = true;
    output5->desiredMode(mode5);
    output5->desiredPos = make_shared<Pos>(11, 12);
    outputs.push_back(output5);

    const shared_ptr<Output> primary = output2;

    stringstream expected;
    expected << "xrandr \\\n";
    expected << " --dpi 123 \\\n";
    expected << " --output One --off \\\n";
    expected << " --output Two --mode 1x2 --rate 3 --pos 5x6 --primary \\\n";
    expected << " --output Three --off \\\n";
    expected << " --output Four --off \\\n";
    expected << " --output Five --mode 8x9 --rate 10 --pos 11x12";

    EXPECT_EQ(expected.str(), renderXrandrCmd(outputs, primary, 123));
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

// TODO: fully test discoverOutputs - probably not going to happen