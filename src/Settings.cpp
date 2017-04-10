#include "Settings.h"

#include <stdexcept>

#include <string.h>
#include <limits.h>

using namespace std;

#define SETTINGS_FILE_NAME ".xLayoutDisplays"
#define SETTINGS_COMMENT_CHAR '#'
#define SETTINGS_SEPS " =\n,"
#define SETTINGS_LINE_MAX 512
#define SETTINGS_KEY_MIRROR "mirror"
#define SETTINGS_KEY_ORDER "order"
#define SETTINGS_KEY_PRIMARY "primary"
#define SETTINGS_KEY_QUIET "quiet"

Settings *Settings::singletonInstance = NULL;

Settings *Settings::instance() {
    if (!singletonInstance)
        singletonInstance = new Settings();
    return singletonInstance;
}

void Settings::loadUserSettings() {
    char settingsFileName[PATH_MAX];
    char line[SETTINGS_LINE_MAX];
    char *key, *val;

    // open settings file for read
    snprintf(settingsFileName, PATH_MAX, "%s/%s", getenv("HOME"), SETTINGS_FILE_NAME);
    FILE *settingsFile = fopen(settingsFileName, "r");
    if (settingsFile) {

        // read each line
        while (fgets(line, SETTINGS_LINE_MAX, settingsFile)) {

            // key
            key = strtok(line, SETTINGS_SEPS);

            // skip comments
            if (key != NULL && key[0] != SETTINGS_COMMENT_CHAR) {

                // value
                val = strtok(NULL, SETTINGS_SEPS);
                if (val == NULL)
                    throw invalid_argument(string() + "missing value for key '" + key + "' in '" + settingsFileName + "'");

                if (strcasecmp(key, SETTINGS_KEY_MIRROR) == 0) {
                    mirror = strcasecmp(val, "true") == 0;
                } else if (strcasecmp(key, SETTINGS_KEY_ORDER) == 0) {
                    while (val) {
                        order.push_back(string(val));
                        val = strtok(NULL, SETTINGS_SEPS);
                    }
                } else if (strcasecmp(key, SETTINGS_KEY_PRIMARY) == 0) {
                    primary = val;
                } else if (strcasecmp(key, SETTINGS_KEY_QUIET) == 0) {
                    verbose = strcasecmp(val, "true") != 0;
                } else {
                    throw invalid_argument(string() + "invalid key '" + key + "' in '" + settingsFileName + "'");
                }
            }
        }
        fclose(settingsFile);
    }
}
