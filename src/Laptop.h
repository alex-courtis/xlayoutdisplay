#ifndef XLAYOUTDISPLAYS_LAPTOP_H
#define XLAYOUTDISPLAYS_LAPTOP_H

#include <string>

// return the prefix that will be used to match embedded displays
const char *embeddedDisplayPrefix();

// todo: don't use a class for the rest of this; a set of functions will suffice
// perhaps maybe not... however we can minimise the statefulness and preserve testing

// singleton to access laptop specific information
class Laptop {
public:
    // singleton accessor
    static Laptop *instance();

    // return true if the lid is closed
    const bool isLidClosed();

    // return true if the display should be disabled due to lid state
    const bool shouldDisableDisplay(const std::string name);

private:
    static Laptop *singletonInstance;

    // set true if we have a "closed" status in the file /proc/acpi/button/lid/.*/state
    void calculateLidClosed(const char *lidRootPath = "/proc/acpi/button/lid");

    bool lidClosed;

    // just for unit testing
    friend class Laptop_shouldDisableDisplay;

    friend class Laptop_calculateLidClosed;

    friend class layout_activateDispls;
};

#endif //XLAYOUTDISPLAYS_LAPTOP_H
