#ifndef XLAYOUTDISPLAYS_SETTINGS_H
#define XLAYOUTDISPLAYS_SETTINGS_H

#include <list>
#include <string>

class Settings {

public:
    // singleton accessor
    static Settings *instance();

    // load user settings from ~/.xLayoutDisplays
    void loadUserSettings();

    bool dryRun = false;
    bool help = false;
    bool info = false;
    bool mirror = false;
    std::list <std::string> order;
    std::string primary;
    bool verbose = true;

private:
    Settings() {}

    void loadUserSettings(const char *settingsFilePath);

    static Settings *singletonInstance;
};

#endif //XLAYOUTDISPLAYS_SETTINGS_H
