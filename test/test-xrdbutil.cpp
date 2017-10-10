#include <gtest/gtest.h>

#include "../src/xrdbutil.h"

using namespace std;

TEST(xrdbutil_renderXrdbCmd, render) {
    EXPECT_EQ("echo \"Xft.dpi: 234\" | xrdb -merge", renderXrdbCmd(234));
}
