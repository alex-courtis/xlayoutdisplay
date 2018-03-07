#ifndef XLAYOUTDISPLAY_MONITORS_H
#define XLAYOUTDISPLAY_MONITORS_H

#include <string>

#define LAPTOP_OUTPUT_PREFIX "eDP"
#define LAPTOP_LID_ROOT_PATH "/proc/acpi/button/lid"

// TODO: just rewrite as a simple static caching function
// calculates and holds state about attached monitors
class Monitors {
public:

    Monitors();

    explicit Monitors(bool laptopLidClosed);

    // return true if the output should be disabled
    const bool shouldDisableOutput(std::string name) const;

    // true if the laptop lid is closed
    const bool laptopLidClosed;
};

// return true if we have a "closed" status under laptopLidRootPath
const bool calculateLaptopLidClosed(const char *laptopLidRootPath);

#endif //XLAYOUTDISPLAY_MONITORS_H
