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
#ifndef XLAYOUTDISPLAY_MONITORS_H
#define XLAYOUTDISPLAY_MONITORS_H

#include <string>

#define LAPTOP_OUTPUT_PREFIX "eDP"
#define LAPTOP_LID_ROOT_PATH "/proc/acpi/button/lid"


// return true if we have a "closed" status under laptopLidRootPath
const bool calculateLaptopLidClosed(const char *laptopLidRootPath);


// calculates and holds state about attached monitors
class Monitors {
public:
    Monitors() : laptopLidClosed(calculateLaptopLidClosed(LAPTOP_LID_ROOT_PATH)) {}

    // return true if the output should be disabled i.e. lid closed and name begins with LAPTOP_OUPUT_PREFIX
    virtual const bool shouldDisableOutput(const std::string &name) const;

    // true if the laptop lid is closed
    const bool laptopLidClosed;
};

#endif //XLAYOUTDISPLAY_MONITORS_H
