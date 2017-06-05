#include <gtest/gtest.h>

#include "../src/Laptop.h"

using namespace std;

class Laptop_calculateLidClosed : public ::testing::Test {
public:
    void createStateFile(const char *contents) {
        ASSERT_EQ(0, mkdir("./lid", 0755));
        ASSERT_EQ(0, mkdir("./lid/LIDX", 0755));
        FILE *lidStateFile = fopen("./lid/LIDX/state", "w");
        ASSERT_TRUE(lidStateFile != NULL);
        fprintf(lidStateFile, contents);
        ASSERT_EQ(0, fclose(lidStateFile));
    };

protected:
    void TearDown() override {
        // always try and remove anything from createStateFile
        remove("./lid/LIDX/state");
        rmdir("./lid/LIDX");
        rmdir("./lid");
    }
};

TEST_F(Laptop_calculateLidClosed, notClosedMissingFile) {
    EXPECT_FALSE(calculateLidClosed("./nonexistent"));
}

TEST_F(Laptop_calculateLidClosed, open) {
    createStateFile("something OpEn something something\n");
    EXPECT_FALSE(calculateLidClosed("./lid"));
}

TEST_F(Laptop_calculateLidClosed, closed) {
    createStateFile("something ClOsEd something something\n");
    EXPECT_TRUE(calculateLidClosed("./lid"));
}


TEST(Laptop_shouldDisableDisplay, matchLidClosed) {
    EXPECT_TRUE(Laptop(true).shouldDisableDisplay(string(LAPTOP_DISPLAY_PREFIX) + "blargh"));
}

TEST(Laptop_shouldDisableDisplay, noMatchLidClosed) {
    EXPECT_FALSE(Laptop(true).shouldDisableDisplay("blargh"));
}

TEST(Laptop_shouldDisableDisplay, matchLidOpen) {
    EXPECT_FALSE(Laptop(false).shouldDisableDisplay(string(LAPTOP_DISPLAY_PREFIX) + "blargh"));
}
