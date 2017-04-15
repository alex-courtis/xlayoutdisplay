#ifndef XLAYOUTDISPLAYS_LAPTOP_H
#define XLAYOUTDISPLAYS_LAPTOP_H

#include <string>

// return true if we have a "closed" status in the file /proc/acpi/button/lid/.*/state
bool lidClosed(const char *lidRootPath = "/proc/acpi/button/lid");

// return true if the display should be disabled due to lid state
bool shouldDisableDisplay(const std::string name, const bool lidClosed);

// return the prefix that will be used to match embedded displays
const char *embeddedDisplayPrefix();

#endif //XLAYOUTDISPLAYS_LAPTOP_H
