/*
   Copyright 2018 Alexander Courtis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/calculations.h"

#include "test-MockEdid.h"
#include "test-MockMonitors.h"

using namespace std;
using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;

TEST(calculations_orderOutputs, reposition) {

    list<shared_ptr<Output>> outputs;
    shared_ptr<Output> output1 = make_shared<Output>("One", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                     shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                     shared_ptr<Edid>());
    outputs.push_back(output1);
    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                     shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                     shared_ptr<Edid>());
    outputs.push_back(output2);
    shared_ptr<Output> output3 = make_shared<Output>("Three", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                     shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                     shared_ptr<Edid>());
    outputs.push_back(output3);
    shared_ptr<Output> output4 = make_shared<Output>("Four", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                     shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                     shared_ptr<Edid>());
    outputs.push_back(output4);
    shared_ptr<Output> output5 = make_shared<Output>("Five", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                     shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                     shared_ptr<Edid>());
    outputs.push_back(output5);

    list<shared_ptr<Output>> orderedOutputs = orderOutputs(outputs, {"FOUR", "THREE", "TWO"});

    EXPECT_EQ(output4, orderedOutputs.front());
    orderedOutputs.pop_front();
    EXPECT_EQ(output3, orderedOutputs.front());
    orderedOutputs.pop_front();
    EXPECT_EQ(output2, orderedOutputs.front());
    orderedOutputs.pop_front();
    EXPECT_EQ(output1, orderedOutputs.front());
    orderedOutputs.pop_front();
    EXPECT_EQ(output5, orderedOutputs.front());
}


class calculations_activateOutputs : public ::testing::Test {
protected:
    shared_ptr<Mode> mode = make_shared<Mode>(0, 0, 0, 0);
    shared_ptr<Pos> pos = make_shared<Pos>(0, 0);
    list<shared_ptr<const Mode>> modes = {mode};
    NiceMock<MockMonitors> mockMonitors;
};

TEST_F(calculations_activateOutputs, primarySpecifiedAndLaptop) {
    list<shared_ptr<Output>> outputs;

    shared_ptr<Output> output1 = make_shared<Output>("One", Output::active, modes, mode, mode, pos, shared_ptr<Edid>());
    outputs.push_back(output1);

    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected, modes, mode, mode, pos,
                                                     shared_ptr<Edid>());
    outputs.push_back(output2);

    shared_ptr<Output> output3 = make_shared<Output>("Three", Output::connected, modes, mode, mode, pos,
                                                     shared_ptr<Edid>());
    outputs.push_back(output3);

    shared_ptr<Output> output4 = make_shared<Output>(LAPTOP_OUTPUT_PREFIX + string("Four"), Output::active, modes, mode,
                                                     mode, pos, shared_ptr<Edid>());
    outputs.push_back(output4);

    EXPECT_CALL(mockMonitors, shouldDisableOutput(_)).WillRepeatedly(Return(false));
    EXPECT_CALL(mockMonitors, shouldDisableOutput(output4->name)).WillOnce(Return(true));

    const shared_ptr<Output> primary = activateOutputs(outputs, "three", mockMonitors);

    EXPECT_TRUE(output1->desiredActive);
    EXPECT_FALSE(output2->desiredActive);
    EXPECT_TRUE(output3->desiredActive);
    EXPECT_FALSE(output4->desiredActive);

    EXPECT_EQ(primary, output3);
}

TEST_F(calculations_activateOutputs, defaultPrimary) {

    list<shared_ptr<Output>> outputs;

    shared_ptr<Output> output1 = make_shared<Output>("One", Output::disconnected, modes, mode, mode, pos,
                                                     shared_ptr<Edid>());
    outputs.push_back(output1);

    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::active, modes, mode, mode, pos,
                                                     shared_ptr<Edid>());
    outputs.push_back(output2);

    shared_ptr<Output> output3 = make_shared<Output>("Three", Output::active, modes, mode, mode, pos,
                                                     shared_ptr<Edid>());
    outputs.push_back(output3);

    const shared_ptr<Output> primary = activateOutputs(outputs, "nouserprimary", mockMonitors);

    EXPECT_FALSE(output1->desiredActive);
    EXPECT_TRUE(output2->desiredActive);
    EXPECT_TRUE(output3->desiredActive);

    EXPECT_EQ(primary, output2);
}

TEST_F(calculations_activateOutputs, noOutputs) {
    EXPECT_THROW(activateOutputs({}, "ouch", mockMonitors), invalid_argument);
}

TEST_F(calculations_activateOutputs, noActiveOrConnected) {
    EXPECT_THROW(
            activateOutputs(
                    {make_shared<Output>("Two", Output::disconnected, modes, mode, mode, pos, shared_ptr<Edid>())},
                    "ouch", mockMonitors),
            runtime_error);
}


TEST(calculations_ltrOutputs, arrange) {

    list<shared_ptr<Output>> outputs;
    list<shared_ptr<const Mode>> modes;

    modes = {make_shared<Mode>(0, 10, 20, 30)};
    shared_ptr<Output> output1 = make_shared<Output>("One", Output::connected, modes, shared_ptr<Mode>(), modes.front(),
                                                     shared_ptr<Pos>(), shared_ptr<Edid>());
    output1->desiredActive = true;
    outputs.push_back(output1);

    modes = {};
    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected, modes, shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    outputs.push_back(output2);

    modes = {make_shared<Mode>(0, 50, 60, 70)};
    shared_ptr<Output> output3 = make_shared<Output>("Three", Output::connected, modes, shared_ptr<Mode>(),
                                                     modes.front(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output3->desiredActive = true;
    outputs.push_back(output3);

    ltrOutputs(outputs);

    EXPECT_TRUE(output1->desiredActive);
    EXPECT_TRUE(output1->desiredMode);
    EXPECT_EQ(10, output1->desiredMode->width);
    EXPECT_EQ(20, output1->desiredMode->height);
    EXPECT_EQ(30, output1->desiredMode->refresh);
    EXPECT_TRUE(output1->desiredPos);
    EXPECT_EQ(0, output1->desiredPos->x);
    EXPECT_EQ(0, output1->desiredPos->y);

    EXPECT_FALSE(output2->desiredActive);
    EXPECT_FALSE(output2->desiredMode);
    EXPECT_FALSE(output2->desiredPos);

    EXPECT_TRUE(output3->desiredActive);
    EXPECT_TRUE(output3->desiredMode);
    EXPECT_EQ(50, output3->desiredMode->width);
    EXPECT_EQ(60, output3->desiredMode->height);
    EXPECT_EQ(70, output3->desiredMode->refresh);
    EXPECT_TRUE(output3->desiredPos);
    EXPECT_EQ(10, output3->desiredPos->x);
    EXPECT_EQ(0, output3->desiredPos->y);
}


TEST(calculations_mirrorOutputs, noneActive) {

    list<shared_ptr<Output>> outputs;

    shared_ptr<Output> output1 = make_shared<Output>("One", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                     shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                     shared_ptr<Edid>());
    outputs.push_back(output1);

    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                     shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                     shared_ptr<Edid>());
    outputs.push_back(output2);

    mirrorOutputs(outputs);

    EXPECT_FALSE(output1->desiredMode);
    EXPECT_FALSE(output1->desiredPos);

    EXPECT_FALSE(output2->desiredMode);
    EXPECT_FALSE(output2->desiredPos);
}

TEST(calculations_mirrorOutputs, oneActive) {

    list<shared_ptr<Output>> outputs;

    const shared_ptr<Mode> mode2 = make_shared<Mode>(0, 5, 6, 0);
    const shared_ptr<Mode> mode1 = make_shared<Mode>(0, 7, 8, 0);

    shared_ptr<Output> output1 = make_shared<Output>("One", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode1, mode2}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output1->desiredActive = true;
    outputs.push_back(output1);

    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                     shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                     shared_ptr<Edid>());
    outputs.push_back(output2);

    mirrorOutputs(outputs);

    EXPECT_EQ(mode1, output1->desiredMode);
    EXPECT_EQ(0, output1->desiredPos->x);
    EXPECT_EQ(0, output1->desiredPos->y);

    EXPECT_FALSE(output2->desiredMode);
    EXPECT_FALSE(output2->desiredPos);
}

TEST(calculations_mirrorOutputs, someActive) {

    list<shared_ptr<Output>> outputs;

    shared_ptr<Mode> mode5 = make_shared<Mode>(0, 1, 2, 0);
    shared_ptr<Mode> mode4 = make_shared<Mode>(0, 1, 2, 1);
    shared_ptr<Mode> mode3 = make_shared<Mode>(0, 3, 4, 0);
    shared_ptr<Mode> mode2 = make_shared<Mode>(0, 5, 6, 0);
    shared_ptr<Mode> mode1 = make_shared<Mode>(0, 7, 8, 0);

    shared_ptr<Output> output1 = make_shared<Output>("One", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode1, mode2}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    outputs.push_back(output1);

    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode3, mode5, mode4}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output2->desiredActive = true;
    outputs.push_back(output2);

    shared_ptr<Output> output3 = make_shared<Output>("Three", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode2, mode5, mode4}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output3->desiredActive = true;
    outputs.push_back(output3);

    mirrorOutputs(outputs);

    EXPECT_FALSE(output1->desiredMode);
    EXPECT_FALSE(output1->desiredPos);

    EXPECT_EQ(mode4, output2->desiredMode);
    EXPECT_EQ(0, output2->desiredPos->x);
    EXPECT_EQ(0, output2->desiredPos->y);

    EXPECT_EQ(mode4, output3->desiredMode);
    EXPECT_EQ(0, output3->desiredPos->x);
    EXPECT_EQ(0, output3->desiredPos->y);
}

TEST(calculations_mirrorOutputs, manyActive) {

    list<shared_ptr<Output>> outputs;

    shared_ptr<Mode> mode4 = make_shared<Mode>(0, 1, 2, 0);
    shared_ptr<Mode> mode3 = make_shared<Mode>(0, 3, 4, 0);
    shared_ptr<Mode> mode2 = make_shared<Mode>(0, 5, 6, 0);
    shared_ptr<Mode> mode1 = make_shared<Mode>(0, 7, 8, 0);

    shared_ptr<Output> output1 = make_shared<Output>("One", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode1, mode2, mode3}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output1->desiredActive = true;
    outputs.push_back(output1);

    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode3, mode4}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output2->desiredActive = true;
    outputs.push_back(output2);

    shared_ptr<Output> output3 = make_shared<Output>("Three", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode2, mode3}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output3->desiredActive = true;
    outputs.push_back(output3);

    mirrorOutputs(outputs);

    EXPECT_EQ(mode3, output1->desiredMode);
    EXPECT_EQ(0, output1->desiredPos->x);
    EXPECT_EQ(0, output1->desiredPos->y);

    EXPECT_EQ(mode3, output2->desiredMode);
    EXPECT_EQ(0, output2->desiredPos->x);
    EXPECT_EQ(0, output2->desiredPos->y);

    EXPECT_EQ(mode3, output3->desiredMode);
    EXPECT_EQ(0, output3->desiredPos->x);
    EXPECT_EQ(0, output3->desiredPos->y);
}

TEST(calculations_mirrorOutputs, noCommon) {

    list<shared_ptr<Output>> outputs;

    shared_ptr<Mode> mode4 = make_shared<Mode>(0, 1, 2, 0);
    shared_ptr<Mode> mode3 = make_shared<Mode>(0, 3, 4, 0);
    shared_ptr<Mode> mode2 = make_shared<Mode>(0, 5, 6, 0);
    shared_ptr<Mode> mode1 = make_shared<Mode>(0, 7, 8, 0);

    shared_ptr<Output> output1 = make_shared<Output>("One", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode1, mode2}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output1->desiredActive = true;
    outputs.push_back(output1);

    shared_ptr<Output> output2 = make_shared<Output>("Two", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode3, mode4}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output2->desiredActive = true;
    outputs.push_back(output2);

    shared_ptr<Output> output3 = make_shared<Output>("Three", Output::disconnected,
                                                     list<shared_ptr<const Mode>>({mode1, mode4}), shared_ptr<Mode>(),
                                                     shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());
    output3->desiredActive = true;
    outputs.push_back(output3);

    EXPECT_THROW(mirrorOutputs(outputs), runtime_error);
}


TEST(calculations_renderUserInfo, renderAll) {
    shared_ptr<Mode> mode1 = make_shared<Mode>(1, 2, 3, 4);
    shared_ptr<Mode> mode2 = make_shared<Mode>(5, 6, 7, 8);
    shared_ptr<Mode> mode3 = make_shared<Mode>(9, 10, 11, 12);
    shared_ptr<Pos> pos = make_shared<Pos>(13, 14);
    shared_ptr<MockEdid> edid1 = make_shared<MockEdid>();
    EXPECT_CALL(*edid1, maxCmHoriz()).WillOnce(Return(15));
    EXPECT_CALL(*edid1, maxCmVert()).WillOnce(Return(16));
    shared_ptr<MockEdid> edid3 = make_shared<MockEdid>();
    EXPECT_CALL(*edid3, maxCmHoriz()).WillOnce(Return(17));
    EXPECT_CALL(*edid3, maxCmVert()).WillOnce(Return(18));

    list<shared_ptr<Output>> outputs;

    shared_ptr<Output> dis = make_shared<Output>("dis", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                 shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(), edid1);
    outputs.push_back(dis);

    shared_ptr<Output> con = make_shared<Output>("con", Output::connected, list<shared_ptr<const Mode>>({mode1, mode2}),
                                                 shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                 shared_ptr<Edid>());
    outputs.push_back(con);

    shared_ptr<Output> act = make_shared<Output>("act", Output::active, list<shared_ptr<const Mode>>({mode3, mode2, mode1}),
                                                 mode2, mode3, pos, edid3);
    outputs.push_back(act);

    const string expected = ""
            "dis disconnected 15cm/16cm\n"
            "con connected\n"
            "   2x3 4Hz\n"
            "  !6x7 8Hz\n"
            "act active 17cm/18cm 6x7+13+14 8Hz\n"
            " +!10x11 12Hz\n"
            "*  6x7 8Hz\n"
            "   2x3 4Hz\n"
            "*current +preferred !optimal";

    EXPECT_EQ(expected, renderUserInfo(outputs));

}

class calculations_calculateOptimalMode : public ::testing::Test {
protected:
    shared_ptr<Mode> mode1 = make_shared<Mode>(1, 2, 3, 4);
    shared_ptr<Mode> mode21 = make_shared<Mode>(5, 6, 7, 8);
    shared_ptr<Mode> mode22 = make_shared<Mode>(5, 6, 7, 9);
    shared_ptr<Mode> mode3 = make_shared<Mode>(9, 10, 11, 12);
    list<shared_ptr<const Mode>> modes = {mode1, mode21, mode22, mode3};
};

TEST_F(calculations_calculateOptimalMode, highestRes) {
    EXPECT_EQ(mode3, calculateOptimalMode(modes, nullptr));
}

TEST_F(calculations_calculateOptimalMode, highestPreferredRefresh) {
    EXPECT_EQ(mode22, calculateOptimalMode(modes, mode21));
}

TEST_F(calculations_calculateOptimalMode, noModes) {
    EXPECT_EQ(nullptr, calculateOptimalMode({}, mode21));
}


class calculations_calculateDpi : public ::testing::Test {
protected:
    const shared_ptr<MockEdid> mockEdid = make_shared<MockEdid>();
    const shared_ptr<Output> output = make_shared<Output>("someoutput", Output::disconnected, list<shared_ptr<const Mode>>(),
                                                          shared_ptr<Mode>(), shared_ptr<Mode>(), shared_ptr<Pos>(),
                                                          mockEdid);
};

TEST_F(calculations_calculateDpi, noOutput) {
    string explaination;
    EXPECT_THROW(calculateDpi(shared_ptr<Output>(), &explaination), invalid_argument);
}

TEST_F(calculations_calculateDpi, noEdid) {
    const shared_ptr<Output> output = make_shared<Output>("noedidoutput", Output::disconnected,
                                                          list<shared_ptr<const Mode>>(), shared_ptr<Mode>(),
                                                          shared_ptr<Mode>(), shared_ptr<Pos>(), shared_ptr<Edid>());

    string explaination;
    const long calculated = calculateDpi(output, &explaination);

    stringstream expectedExplaination;
    expectedExplaination << "DPI defaulting to "
                         << DEFAULT_DPI
                         << "; EDID information not available for output "
                         << output->name;

    EXPECT_EQ(DEFAULT_DPI, calculated);
    EXPECT_EQ(expectedExplaination.str(), explaination);
}

TEST_F(calculations_calculateDpi, noDesiredMode) {

    string explaination;
    const long calculated = calculateDpi(output, &explaination);

    stringstream expectedExplaination;
    expectedExplaination << "DPI defaulting to "
                         << DEFAULT_DPI
                         << "; no desired mode for output "
                         << output->name;

    EXPECT_EQ(DEFAULT_DPI, calculated);
    EXPECT_EQ(expectedExplaination.str(), explaination);
}

TEST_F(calculations_calculateDpi, zeroEdid) {
    output->desiredMode = make_shared<const Mode>(1, 2, 3, 4);
    EXPECT_CALL(*mockEdid, dpiForMode(output->desiredMode)).WillOnce(Return(0));

    string explaination;
    const long calculated = calculateDpi(output, &explaination);

    stringstream expectedExplaination;
    expectedExplaination << "DPI defaulting to "
                         << DEFAULT_DPI
                         << " as no EDID horiz/vert cm information available for "
                         << output->name
                         << "; this is normal for projectors and some TVs";

    EXPECT_EQ(DEFAULT_DPI, calculated);
    EXPECT_EQ(expectedExplaination.str(), explaination);
}

TEST_F(calculations_calculateDpi, valid) {
    output->desiredMode = make_shared<const Mode>(1, 2, 3, 4);
    EXPECT_CALL(*mockEdid, dpiForMode(output->desiredMode)).WillOnce(Return(1));

    string explaination;
    const long calculated = calculateDpi(output, &explaination);

    stringstream expectedExplaination;
    expectedExplaination << "calculated DPI "
                         << 1
                         << " for output "
                         << output->name;

    EXPECT_EQ(1, calculated);
    EXPECT_EQ(expectedExplaination.str(), explaination);
}