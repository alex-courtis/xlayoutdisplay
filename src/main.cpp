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

        // arrange left to right
        ltrDispls(displs);

        // render desired state for xrandr
        const string xrandr = renderCmd(displs);
        if (settings->verbose || settings->dryRun)
            printf("\n%s\n", xrandr.c_str());

        if (settings->dryRun)
            return EXIT_SUCCESS;

        // invoke xrandr
        return system(xrandr.c_str());

    } catch (const exception &e) {
        fprintf(stderr, "FAIL: %s, exiting\n", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        fprintf(stderr, "EPIC FAIL: unknown exception, exiting\n");
        return EXIT_FAILURE;
    }
}
