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
#include "Monitors.h"

#include <cstring>
#include <dirent.h>

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

const bool Monitors::shouldDisableOutput(const std::string &name) const {
    return laptopLidClosed && strncasecmp(LAPTOP_OUTPUT_PREFIX, name.c_str(), strlen(LAPTOP_OUTPUT_PREFIX)) == 0;
}
