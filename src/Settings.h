#ifndef XLAYOUTDISPLAYS_SETTINGS_H
#define XLAYOUTDISPLAYS_SETTINGS_H

#include <list>
#include <string>

// user provided settings for this utility
class Settings {
public:

    // construct settings with user provided arguments, overriding with ~/.xLayoutDisplays
    Settings(const int argc, char *const *argv);

    bool dryRun = false;
    bool help = false;
    bool info = false;
    bool mirror = false;
    std::list<std::string> order;
    std::string primary;
    bool verbose = true;

private:
    // unit testing constructor
    Settings() {};

    // load the user settings from the path provided
    void loadUserSettings(const std::string settingsFilePath);

    // load settings via getopts
    void loadCliSettings(const int argc, char *const *argv);

    // for unit testing
    friend class Settings_loadUserSettings;
};

#endif //XLAYOUTDISPLAYS_SETTINGS_H
