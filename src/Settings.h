#ifndef XLAYOUTDISPLAY_SETTINGS_H
#define XLAYOUTDISPLAY_SETTINGS_H

#include <list>
#include <string>

// user provided settings for this utility
class Settings {
public:
    Settings() = default;

    // default settings, overridden with ~/.xLayoutDisplay then argc/argv
    Settings(int argc, char **argv);

    bool dryRun = false;
    bool help = false;
    bool info = false;
    bool mirror = false;
    std::list<std::string> order;
    std::string primary;
    bool verbose = true;

    // override settings via getopts
    void loadCliSettings(int argc, char **argv);

    // override settings from the absolute path provided
    // throws invalid_argument:
    //   invalid key
    //   key missing a value
    void loadUserSettings(std::string settingsFilePath);
};

#endif //XLAYOUTDISPLAY_SETTINGS_H
