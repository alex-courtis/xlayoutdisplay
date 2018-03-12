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
    virtual const bool shouldDisableOutput(std::string name) const;

    // true if the laptop lid is closed
    const bool laptopLidClosed;
};

#endif //XLAYOUTDISPLAY_MONITORS_H
