#include <gtest/gtest.h>

#include "../src/xrdbutil.h"
#include "../src/Displ.h"

using namespace std;

TEST(xrdbutil_renderXrdbCmd, render) {
    Displ::desiredDpi = 123;

    EXPECT_EQ("echo \"Xft.dpi: 123\" | xrdb -merge", renderXrdbCmd());
}
