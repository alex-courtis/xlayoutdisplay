#include <gtest/gtest.h>

#include "../src/Settings.h"

using namespace std;

const char *settingsFileName = "./availableSettings";
FILE *settingsFile;

void writeUserSettings(const char *settingsFileContents) {
    ASSERT_TRUE((settingsFile = fopen(settingsFileName, "w")) != NULL);
    fprintf(settingsFile, settingsFileContents);
    ASSERT_EQ(0, fclose(settingsFile));
}

TEST(Settings_loadUserSettings, AvailableSettingsAndComments) {
    writeUserSettings(""
            "\n"
            "#\n"
            " #\n"
            " #bleh\n"
            " MirroR = tRue  \n"
            "#mirror=false\n"
            " #mirror=false\n"
            "quiet=true\n"
            " OrDeR = one , TwO  \n"
            "prImAry =    blargh");

    Settings settings;
    settings.loadUserSettings(settingsFileName);

    // unavailable settings
    EXPECT_FALSE(settings.dryRun);
    EXPECT_FALSE(settings.help);
    EXPECT_FALSE(settings.info);

    // selected settings
    EXPECT_TRUE(settings.mirror);
    EXPECT_FALSE(settings.verbose);
    EXPECT_EQ(settings.order, list<string>({"one", "TwO"}));
    EXPECT_EQ(settings.primary, "blargh");
}

TEST(Settings_loadUserSettings, InvalidKey) {
    writeUserSettings("bleh=blargh");

    EXPECT_THROW(Settings().loadUserSettings(settingsFileName), invalid_argument);
}

TEST(Settings_loadUserSettings, MissingValue) {
    writeUserSettings("bleh=");

    EXPECT_THROW(Settings().loadUserSettings(settingsFileName), invalid_argument);
}
