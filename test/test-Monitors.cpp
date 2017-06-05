#include <gtest/gtest.h>

#include "../src/Monitors.h"

using namespace std;

class Monitors_calculateLaptopLidClosed : public ::testing::Test {
protected:
    void TearDown() override {
        // always try and remove anything from createStateFile
        remove("./lid/LIDX/state");
        rmdir("./lid/LIDX");
        rmdir("./lid");
    }

    void createStateFile(const char *contents) {
        ASSERT_EQ(0, mkdir("./lid", 0755));
        ASSERT_EQ(0, mkdir("./lid/LIDX", 0755));
        FILE *lidStateFile = fopen("./lid/LIDX/state", "w");
        ASSERT_TRUE(lidStateFile != NULL);
        fprintf(lidStateFile, contents);
        ASSERT_EQ(0, fclose(lidStateFile));
    };
};

TEST_F(Monitors_calculateLaptopLidClosed, notClosedMissingFile) {
    EXPECT_FALSE(calculateLaptopLidClosed("./nonexistent"));
}

TEST_F(Monitors_calculateLaptopLidClosed, open) {
    createStateFile("something OpEn something something\n");
    EXPECT_FALSE(calculateLaptopLidClosed("./lid"));
}

TEST_F(Monitors_calculateLaptopLidClosed, closed) {
    createStateFile("something ClOsEd something something\n");
    EXPECT_TRUE(calculateLaptopLidClosed("./lid"));
}


TEST(Laptop_shouldDisableDisplay, matchLidClosed) {
    EXPECT_TRUE(Monitors(true).shouldDisableDisplay(string(LAPTOP_DISPLAY_PREFIX) + "blargh"));
}

TEST(Laptop_shouldDisableDisplay, noMatchLidClosed) {
    EXPECT_FALSE(Monitors(true).shouldDisableDisplay("blargh"));
}

TEST(Laptop_shouldDisableDisplay, matchLidOpen) {
    EXPECT_FALSE(Monitors(false).shouldDisableDisplay(string(LAPTOP_DISPLAY_PREFIX) + "blargh"));
}
