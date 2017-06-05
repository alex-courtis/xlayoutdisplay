#ifndef XLAYOUTDISPLAYS_SETTINGS_H
#define XLAYOUTDISPLAYS_SETTINGS_H

#include <list>
#include <string>

// user provided settings for this utility
class Settings {
public:

    // construct settings from ~/.xLayoutDisplays, overriding with with user provided arguments
    Settings(int argc, char **argv);

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

    // load the user settings from the absolute path provided
    void loadUserSettings(const std::string settingsFilePath);

    // load settings via getopts
    void loadCliSettings(int argc, char **argv);

    // for unit testing
    // todo: no more friends please
    friend class Settings_loadUserSettings;
};

#endif //XLAYOUTDISPLAYS_SETTINGS_H
