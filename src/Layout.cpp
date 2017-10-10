#include "Layout.h"
#include "xrandrrutil.h"
#include "xrdbutil.h"
#include "calculations.h"

using namespace std;

Layout::Layout(int argc, char **argv) :
        displs(discoverDispls()),
        settings(Settings(argc, argv)) {
}

const int Layout::apply() {

    // output verbose information
    if (settings.verbose || settings.info)
        printf("%s\n\nlaptop lid %s\n", renderUserInfo(displs).c_str(),
               monitors.laptopLidClosed ? "closed" : "open or not present");

    // current info is all output, we're done
    if (settings.info)
        return EXIT_SUCCESS;

    // determine desired state
    orderDispls(displs, settings.order);
    activateDispls(displs, settings.primary, monitors);

    // arrange mirrored or left to right
    if (settings.mirror)
        mirrorDispls(displs);
    else
        ltrDispls(displs);

    // determine DPI for all displays
    string dpiExplaination = calculateDpi(displs);
    if (settings.verbose)
        printf("\n%s\n", dpiExplaination.c_str());

    // render desired commands
    const string xrandrCmd = renderXrandrCmd(displs);
    const string xrdbCmd = renderXrdbCmd();
    if (settings.verbose || settings.dryRun)
        printf("\n\n%s\n\n%s\n", xrandrCmd.c_str(), xrdbCmd.c_str());

    // execute
    if (!settings.dryRun) {
        int rc = system(xrandrCmd.c_str());
        if (rc != 0)
            return rc;
        rc = system(xrdbCmd.c_str());
        if (rc != 0)
            return rc;
    }
    return EXIT_SUCCESS;
}
