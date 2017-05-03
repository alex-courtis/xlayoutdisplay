#include <gtest/gtest.h>
#include "../src/Laptop.h"

using namespace std;

void createStateFile(const char *contents) {
    ASSERT_EQ(0, mkdir("./lid", 0755));
    ASSERT_EQ(0, mkdir("./lid/LIDX", 0755));
    FILE *lidStateFile = fopen("./lid/LIDX/state", "w");
    ASSERT_TRUE(lidStateFile != NULL);
    fprintf(lidStateFile, contents);
    ASSERT_EQ(0, fclose(lidStateFile));
}

// always try and remove
void removeStateFile() {
    remove("./lid/LIDX/state");
    rmdir("./lid/LIDX");
    rmdir("./lid");
}

class Laptop_calculateLidClosed : public ::testing::Test {
protected:
    void TearDown() override {
        Laptop::singletonInstance = NULL;
    }

    void calculateLidClosed(const char *lidRootPath) {
        Laptop::instance()->calculateLidClosed(lidRootPath);
    }
};

TEST_F(Laptop_calculateLidClosed, notClosedMissingFile) {
    calculateLidClosed("./nonexistent");

    EXPECT_FALSE(Laptop::instance()->isLidClosed());
}

TEST_F(Laptop_calculateLidClosed, open) {
    createStateFile("something OpEn something something\n");

    calculateLidClosed("./lid");
    EXPECT_FALSE(Laptop::instance()->isLidClosed());

    removeStateFile();
}

TEST_F(Laptop_calculateLidClosed, closed) {
    createStateFile("something ClOsEd something something\n");

    calculateLidClosed("./lid");
    EXPECT_TRUE(Laptop::instance()->isLidClosed());

    removeStateFile();
}


class Laptop_shouldDisableDisplay : public ::testing::Test {
protected:
    void setLidClosed(const bool closed) {
        Laptop::instance()->lidClosed = closed;
    }
};

TEST_F(Laptop_shouldDisableDisplay, matchLidClosed) {
    setLidClosed(true);
    EXPECT_TRUE(Laptop::instance()->shouldDisableDisplay(string(embeddedDisplayPrefix()) + "blargh"));
}

TEST_F(Laptop_shouldDisableDisplay, noMatchLidClosed) {
    setLidClosed(true);
    EXPECT_FALSE(Laptop::instance()->shouldDisableDisplay("blargh"));
}

TEST_F(Laptop_shouldDisableDisplay, matchLidOpen) {
    setLidClosed(false);
    EXPECT_FALSE(Laptop::instance()->shouldDisableDisplay(string(embeddedDisplayPrefix()) + "blargh"));
}
