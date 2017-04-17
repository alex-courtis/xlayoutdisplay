#ifndef XLAYOUTDISPLAYS_SETTINGS_H
#define XLAYOUTDISPLAYS_SETTINGS_H

#include <list>
#include <string>

// singleton representing settings of this utility
class Settings {

public:
    // singleton accessor
    static Settings *instance();

    // load user settings from ~/.xLayoutDisplays
    void loadUserSettings();

    // load settings via getopts
    void loadCliSettings(int argc, char **argv);

    bool dryRun = false;
    bool help = false;
    bool info = false;
    bool mirror = false;
    std::list<std::string> order;
    std::string primary;
    bool verbose = true;

private:
    Settings() {}

    void loadUserSettings(const char *settingsFilePath);

    static Settings *singletonInstance;

    // for unit testing
    friend class Settings_loadUserSettings;
};

#endif //XLAYOUTDISPLAYS_SETTINGS_H
