#include <gtest/gtest.h>

#include "../src/laptop.h"

// fail on creation of the state file
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

TEST(LidTest, LidNotClosedMissingFile) {
    EXPECT_FALSE(isLidClosed("./nonexistent"));
}

TEST(LidTest, LidOpen) {
    createStateFile("something OpEn something something\n");

    EXPECT_FALSE(isLidClosed("./lid"));

    removeStateFile();
}

TEST(LidTest, LidClosed) {
    createStateFile("something ClOsEd something something\n");

    EXPECT_TRUE(isLidClosed("./lid"));

    removeStateFile();
}
