#ifndef XLAYOUTDISPLAY_TEST_MOCKMONITORS_H
#define XLAYOUTDISPLAY_TEST_MOCKMONITORS_H

#include <gmock/gmock.h>

#include "../src/Monitors.h"

class MockMonitors : public Monitors {
public:
    MOCK_CONST_METHOD1(shouldDisableOutput, const bool(const std::string &name));
};

#endif //XLAYOUTDISPLAY_TEST_MOCKMONITORS_H
