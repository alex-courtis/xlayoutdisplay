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

#include "../src/Mode.h"

using namespace std;

TEST(Mode_order, width) {
    EXPECT_TRUE(Mode(0, 1, 2, 2) < Mode(0, 2, 1, 1));
}

TEST(Mode_order, height) {
    EXPECT_TRUE(Mode(0, 1, 1, 2) < Mode(0, 1, 2, 1));
}

TEST(Mode_order, refresh) {
    EXPECT_TRUE(Mode(0, 1, 1, 1) < Mode(0, 1, 1, 2));
}
