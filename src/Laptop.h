#ifndef XLAYOUTDISPLAYS_LAPTOP_H
#define XLAYOUTDISPLAYS_LAPTOP_H

#include <string>

#define LAPTOP_DISPLAY_PREFIX "eDP"
#define LAPTOP_LID_ROOT_PATH "/proc/acpi/button/lid"

// calculates and holds state about laptop specifics
class Laptop {
public:

    // construct with lidClosed based on calculateLidClosed(LAPTOP_LID_ROOT_PATH)
    Laptop();

    Laptop(const bool lidClosed);

    // return true if the display should be disabled
    const bool shouldDisableDisplay(const std::string name) const;

    // true if the laptop lid is closed
    const bool lidClosed;
};

// return true if we have a "closed" status under laptopLidRootPath
const bool calculateLidClosed(const char *laptopLidRootPath);

#endif //XLAYOUTDISPLAYS_LAPTOP_H
