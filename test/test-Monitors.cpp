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
        ASSERT_TRUE(lidStateFile != nullptr);
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
