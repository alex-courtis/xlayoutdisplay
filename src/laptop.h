#ifndef XLAYOUTDISPLAYS_LAPTOP_H
#define XLAYOUTDISPLAYS_LAPTOP_H

// return true if we have a "closed" status in the file /proc/acpi/button/lid/???/state
bool isLidClosed(const char *lidRootPath = "/proc/acpi/button/lid");

#endif //XLAYOUTDISPLAYS_LAPTOP_H
