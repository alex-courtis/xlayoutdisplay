#include "Settings.h"
#include "Laptop.h"

#include "xrandrrutil.h"
#include "layout.h"

using namespace std;

int main(int argc, char **argv) {
    try {
        Settings *settings = Settings::instance();

        // load persistent settings
        settings->loadUserSettings();

        // override with CLI settings
        settings->loadCliSettings(argc, argv);

        // discover current state
        list <DisplP> displs = discoverDispls();

        // output verbose information
        if (settings->verbose || settings->info)
            printf("%s\n\nlid %s\n", renderUserInfo(displs).c_str(), Laptop::instance()->isLidClosed() ? "closed" : "open or not present");

        // current info is all output, we're done
        if (settings->info)
            return EXIT_SUCCESS;

        // determine desired state
        orderDispls(displs, settings->order);
        activateDispls(displs, settings->primary);

        // arrange mirrored or left to right (default)
        if (settings->mirror)
            mirrorDispls(displs);
        else
            ltrDispls(displs);

        // render desired cmd for xrandr
        const string xrandrCmd = renderCmd(displs);
        if (settings->verbose || settings->dryRun)
            printf("\n%s\n", xrandrCmd.c_str());

        // execute xrandr, returning its return
        if (settings->dryRun)
            return EXIT_SUCCESS;
        else
            return system(xrandrCmd.c_str());

    } catch (const exception &e) {
        fprintf(stderr, "FAIL: %s, exiting\n", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        fprintf(stderr, "EPIC FAIL: unknown exception, exiting\n");
        return EXIT_FAILURE;
    }
}
