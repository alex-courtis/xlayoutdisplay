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

        // reset Settings singleton
        Settings::singletonInstance = NULL;

        // always remove the file
        ASSERT_EQ(0, remove(settingsFileName));
    }

    void loadUserSettings(const char *settingsFilePath) {
        Settings::instance()->loadUserSettings(settingsFilePath);
    }

    const char *settingsFileName = "./availableSettings";
    FILE *settingsFile;
};

TEST_F(Settings_loadUserSettings, DefaultsFromEmpty) {
    EXPECT_FALSE(Settings::instance()->dryRun);
    EXPECT_FALSE(Settings::instance()->help);
    EXPECT_FALSE(Settings::instance()->info);
    EXPECT_FALSE(Settings::instance()->mirror);
    EXPECT_TRUE(Settings::instance()->primary.empty());
    EXPECT_TRUE(Settings::instance()->order.empty());
    EXPECT_TRUE(Settings::instance()->verbose);
}

TEST_F(Settings_loadUserSettings, DefaultsFromMissing) {
    loadUserSettings("blargh");

    EXPECT_FALSE(Settings::instance()->dryRun);
    EXPECT_FALSE(Settings::instance()->help);
    EXPECT_FALSE(Settings::instance()->info);
    EXPECT_FALSE(Settings::instance()->mirror);
    EXPECT_TRUE(Settings::instance()->primary.empty());
    EXPECT_TRUE(Settings::instance()->order.empty());
    EXPECT_TRUE(Settings::instance()->verbose);
}

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

    loadUserSettings("./availableSettings");

    // unavailable settings
    EXPECT_FALSE(Settings::instance()->dryRun);
    EXPECT_FALSE(Settings::instance()->help);
    EXPECT_FALSE(Settings::instance()->info);

    // selected settings
    EXPECT_TRUE(Settings::instance()->mirror);
    EXPECT_FALSE(Settings::instance()->verbose);
    EXPECT_EQ(Settings::instance()->order, list<string>({"one", "TwO"}));
    EXPECT_EQ(Settings::instance()->primary, "blargh");
}

TEST_F(Settings_loadUserSettings, InvalidKey) {

    fprintf(settingsFile, "bleh=blargh");
    ASSERT_EQ(0, fclose(settingsFile));

    EXPECT_THROW(loadUserSettings("./availableSettings"), invalid_argument);
}

TEST_F(Settings_loadUserSettings, MissingValue) {

    fprintf(settingsFile, "quiet=");
    ASSERT_EQ(0, fclose(settingsFile));

    EXPECT_THROW(loadUserSettings("./availableSettings"), invalid_argument);
}
