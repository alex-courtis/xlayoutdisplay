#include <gtest/gtest.h>

#include "../src/Settings.h"

using namespace std;

class Settings_loadUserSettings : public ::testing::Test {
protected:
    virtual void SetUp() {
        // open the file for write; test must close before read
        ASSERT_TRUE((settingsFile = fopen(settingsFileName, "w")) != NULL);
    }

    virtual void TearDown() {
        // always remove the file
        ASSERT_EQ(0, remove(settingsFileName));
    }

    void loadUserSettings() {
        settings.loadUserSettings(settingsFileName);
    }

    Settings settings;
    const char *settingsFileName = "./availableSettings";
    FILE *settingsFile;
};

TEST_F(Settings_loadUserSettings, AvailableSettingsAndComments) {

    fprintf(settingsFile, ""
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
    ASSERT_EQ(0, fclose(settingsFile));

    loadUserSettings();

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

TEST_F(Settings_loadUserSettings, InvalidKey) {

    fprintf(settingsFile, "bleh=blargh");
    ASSERT_EQ(0, fclose(settingsFile));

    EXPECT_THROW(loadUserSettings(), invalid_argument);
}

TEST_F(Settings_loadUserSettings, MissingValue) {

    fprintf(settingsFile, "quiet=");
    ASSERT_EQ(0, fclose(settingsFile));

    EXPECT_THROW(loadUserSettings(), invalid_argument);
}
