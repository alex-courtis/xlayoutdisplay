#include "layout.h"
#include "xrandrrutil.h"
#include "xrdbutil.h"
#include "calculations.h"
#include "Settings.h"

using namespace std;

const int layout(int argc, char **argv) {

    // load settings
    const Settings settings = Settings(argc, argv);

    // discover monitors
    const Monitors monitors = Monitors();

    // discover displays
    list<shared_ptr<Displ>> displs = discoverDispls();
    if (displs.empty())
        throw runtime_error("no displays found");

    // output verbose information
    if (settings.verbose || settings.info)
        printf("%s\n\nlaptop lid %s\n", renderUserInfo(displs).c_str(),
               monitors.laptopLidClosed ? "closed" : "open or not present");

    // current info is all output, we're done
    if (settings.info)
        return EXIT_SUCCESS;

    // determine desired state
    orderDispls(displs, settings.order);
    const shared_ptr<Displ> primary = activateDispls(displs, settings.primary, monitors);

    // arrange mirrored or left to right
    if (settings.mirror)
        mirrorDispls(displs);
    else
        ltrDispls(displs);

    // determine DPI from the primary
    string dpiExplaination;
    long dpi = calculateDpi(primary, dpiExplaination);
    if (settings.verbose)
        printf("\n\n%s\n", dpiExplaination.c_str());

    // render desired commands
    const string xrandrCmd = renderXrandrCmd(displs, primary, dpi);
    const string xrdbCmd = renderXrdbCmd(dpi);
    if (settings.verbose || settings.dryRun)
        printf("\n%s\n\n%s\n", xrandrCmd.c_str(), xrdbCmd.c_str());

    // execute
    if (!settings.dryRun) {
        // xrandr
        int rc = system(xrandrCmd.c_str());
        if (rc != 0)
            return rc;

        // TODO: extract function as below or do something different :shrug:
        // clear root pointer to refresh
        Display *dpy = XOpenDisplay(nullptr);
        int screen = DefaultScreen(dpy);
        Window root = RootWindow(dpy, screen);
        XUndefineCursor(dpy, root);
        XCloseDisplay(dpy);

        // xrdb
        rc = system(xrdbCmd.c_str());
        if (rc != 0)
            return rc;
    }
    return EXIT_SUCCESS;
}
