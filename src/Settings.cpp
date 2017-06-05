#include "Settings.h"
#include "Monitors.h"
#include "util.h"

#include <string.h>
#include <getopt.h>

using namespace std;

#define USAGE "Usage: %s [-h] [-i] [-n] [-o order] [-p primary] [-q]\n"

#define SETTINGS_FILE_COMMENT_CHAR '#'
#define SETTINGS_FILE_SEPS " =\n,"
#define SETTINGS_FILE_LINE_MAX 512
#define SETTINGS_FILE_KEY_MIRROR "mirror"
#define SETTINGS_FILE_KEY_ORDER "order"
#define SETTINGS_FILE_KEY_PRIMARY "primary"
#define SETTINGS_FILE_KEY_QUIET "quiet"

Settings::Settings(int argc, char **argv) {

    // load persistent settings from ~/.xLayoutDisplays
    loadUserSettings(resolveTildePath(".xLayoutDisplays"));

    // override with CLI settings
    loadCliSettings(argc, argv);
}

// display help and exit with success
void printHelp(char *progPath) {
    char *progName = basename(progPath);
    printf(""
                   "Detects and arranges displays in a left to right manner.\n"
                   "Invokes xrandr to perform arrangement.\n"
                   "Highest resolution and refresh are used for each display.\n"
                   "Displays starting with \"%s\" are disabled if the laptop lid is closed as per /proc/acpi/button/lid/.*/state\n"
                   "Displays are ordered via Xrandr default.\n"
                   "The first display will be primary unless -p specified.\n"
                   "\n", LAPTOP_DISPLAY_PREFIX
    );
    printf(USAGE, progName);
    printf(""
                   "  -h  display this help text and exit\n"
                   "  -i  display information about current displays and exit\n"
                   "  -m  mirror displays using the lowest common resolution\n"
                   "  -n  perform a trial run and exit\n"
                   "  -o  order of displays, space/comma delimited\n"
                   "  -p  primary display\n"
                   "  -q  suppress output\n"
    );
    printf("\n"
                   "e.g.: %s -o DP-0,HDMI-0 -p HDMI-0\n"
                   "  arranges DP-0 left, HDMI-0 right, with any remaining displays further right, with HDMI-0 as primary\n"
                   "", progName
    );
    exit(EXIT_SUCCESS);
}

// display usage and help hint then exit with failure
void usage(char *progPath) {
    char *progName = basename(progPath);
    fprintf(stderr, USAGE, progName);
    fprintf(stderr, "Try '%s -h' for more information.\n", progName);
    exit(EXIT_FAILURE);
}

// no, this does not get any tests
void Settings::loadCliSettings(int argc, char **argv) {

    // load command line settings
    int opt;
    while ((opt = getopt(argc, argv, "himno:p:q")) != -1) {
        switch (opt) {
            case 'h':
                help = true;
                break;
            case 'i':
                info = true;
                break;
            case 'm':
                mirror = true;
                break;
            case 'n':
                dryRun = true;
                break;
            case 'o':
                for (char *token = strtok(optarg, " ,"); token != NULL; token = strtok(NULL, " ,"))
                    order.push_back(string(token));
                break;
            case 'p':
                primary = optarg;
                break;
            case 'q':
                verbose = false;
                break;
            default:
                usage(argv[0]);
        }
    }
    if (argc > optind)
        usage(argv[0]);
    if (help)
        printHelp(argv[0]);
}

void Settings::loadUserSettings(const string settingsFilePath) {
    char line[SETTINGS_FILE_LINE_MAX];
    char *key, *val;

    // read settings file, if it exists
    FILE *settingsFile = fopen(settingsFilePath.c_str(), "r");
    if (settingsFile) {

        // read each line
        while (fgets(line, SETTINGS_FILE_LINE_MAX, settingsFile)) {

            // key
            key = strtok(line, SETTINGS_FILE_SEPS);

            // skip comments
            if (key != NULL && key[0] != SETTINGS_FILE_COMMENT_CHAR) {

                // value
                val = strtok(NULL, SETTINGS_FILE_SEPS);
                if (val == NULL)
                    throw invalid_argument(
                            string() + "missing value for key '" + key + "' in '" + settingsFilePath + "'");

                if (strcasecmp(key, SETTINGS_FILE_KEY_MIRROR) == 0) {
                    mirror = strcasecmp(val, "true") == 0;
                } else if (strcasecmp(key, SETTINGS_FILE_KEY_ORDER) == 0) {
                    while (val) {
                        order.push_back(string(val));
                        val = strtok(NULL, SETTINGS_FILE_SEPS);
                    }
                } else if (strcasecmp(key, SETTINGS_FILE_KEY_PRIMARY) == 0) {
                    primary = val;
                } else if (strcasecmp(key, SETTINGS_FILE_KEY_QUIET) == 0) {
                    verbose = strcasecmp(val, "true") != 0;
                } else {
                    throw invalid_argument(string() + "invalid key '" + key + "' in '" + settingsFilePath + "'");
                }
            }
        }
        fclose(settingsFile);
    }
}
