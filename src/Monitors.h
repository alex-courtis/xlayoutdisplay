#ifndef XLAYOUTDISPLAYS_MONITORS_H
#define XLAYOUTDISPLAYS_MONITORS_H

#include <string>

#define LAPTOP_DISPLAY_PREFIX "eDP"
#define LAPTOP_LID_ROOT_PATH "/proc/acpi/button/lid"

// calculates and holds state about attached monitors
class Monitors {
public:

    // TODO: better constructor; perhaps move calculateLaptopLidClosed to calculations or laptop and make this a simple true/false
    // laptopLidClosed = calculateLaptopLidClosed(LAPTOP_LID_ROOT_PATH)
    Monitors();

    explicit Monitors(bool laptopLidClosed);

    // return true if the display should be disabled
    const bool shouldDisableDisplay(std::string name) const;

    // true if the laptop lid is closed
    const bool laptopLidClosed;
};

// return true if we have a "closed" status under laptopLidRootPath
const bool calculateLaptopLidClosed(const char *laptopLidRootPath);

#endif //XLAYOUTDISPLAYS_MONITORS_H
