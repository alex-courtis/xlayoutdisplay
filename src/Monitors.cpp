#include "Monitors.h"

#include <cstring>
#include <dirent.h>

Monitors::Monitors() : laptopLidClosed(calculateLaptopLidClosed(LAPTOP_LID_ROOT_PATH)) {}

Monitors::Monitors(const bool laptopLidClosed) : laptopLidClosed(laptopLidClosed) {}

const bool Monitors::shouldDisableDisplay(const std::string name) const {
    return laptopLidClosed && strncasecmp(LAPTOP_DISPLAY_PREFIX, name.c_str(), strlen(LAPTOP_DISPLAY_PREFIX)) == 0;
}

const bool calculateLaptopLidClosed(const char *laptopLidRootPath) {
    static char lidFileName[PATH_MAX];
    static char line[512];

    // find the lid state directory
    DIR *dir = opendir(laptopLidRootPath);
    if (dir) {
        struct dirent *dirent;
        while ((dirent = readdir(dir)) != nullptr) {
            if (dirent->d_type == DT_DIR && strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {

                // read the lid state file
                snprintf(lidFileName, PATH_MAX, "%s/%s/%s", laptopLidRootPath, dirent->d_name, "state");
                FILE *lidFile = fopen(lidFileName, "r");
                if (lidFile != nullptr) {
                    if (fgets(line, 512, lidFile))
                        if (strcasestr(line, "closed"))
                            return true;
                    fclose(lidFile);
                }

                // drivers/acpi/button.c acpi_button_add_fs seems to indicate there will be only one file
                break;
            }
        }
        closedir(dir);
    }
    return false;
}

