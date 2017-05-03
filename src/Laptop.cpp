#include "Laptop.h"

#include <string.h>
#include <dirent.h>

#define EMBEDDED_DISPLAY_PREFIX "eDP"

const char *embeddedDisplayPrefix() {
    return EMBEDDED_DISPLAY_PREFIX;
}

Laptop *Laptop::singletonInstance = NULL;

Laptop *Laptop::instance() {
    if (!singletonInstance) {
        singletonInstance = new Laptop();
        singletonInstance->calculateLidClosed();
    }
    return singletonInstance;
}

const bool Laptop::isLidClosed() {
    return lidClosed;
}

const bool Laptop::shouldDisableDisplay(const std::string name) {
    return lidClosed && strncasecmp(EMBEDDED_DISPLAY_PREFIX, name.c_str(), strlen(EMBEDDED_DISPLAY_PREFIX)) == 0;
}

void Laptop::calculateLidClosed(const char *lidRootPath) {
    lidClosed = false;

    static char lidFileName[PATH_MAX];
    static char line[512];

    // find the lid state directory
    DIR *dir = opendir(lidRootPath);
    if (dir) {
        struct dirent *dirent;
        while ((dirent = readdir(dir)) != NULL) {
            if (dirent->d_type == DT_DIR && strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {

                // read the lid state file
                snprintf(lidFileName, PATH_MAX, "%s/%s/%s", lidRootPath, dirent->d_name, "state");
                FILE *lidFile = fopen(lidFileName, "r");
                if (lidFile != NULL) {
                    if (fgets(line, 512, lidFile))
                        if (strcasestr(line, "closed"))
                            lidClosed = true;
                    fclose(lidFile);
                }

                // drivers/acpi/button.c acpi_button_add_fs seems to indicate there will be only one file
                break;
            }
        }
        closedir(dir);
    }
}
