#ifndef XLAYOUTDISPLAYS_SETTINGS_H
#define XLAYOUTDISPLAYS_SETTINGS_H

#include <list>
#include <string>

// user provided settings for this utility
class Settings {
public:
    Settings() {};

    // default settings, overridden with ~/.xLayoutDisplays then argc/argv
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
    void loadUserSettings(const std::string settingsFilePath);
};

#endif //XLAYOUTDISPLAYS_SETTINGS_H
