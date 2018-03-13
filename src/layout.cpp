#include <X11/Xcursor/Xcursor.h>
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

    // discover outputs
    list<shared_ptr<Output>> outputs = discoverOutputs();
    if (outputs.empty())
        throw runtime_error("no outputs found");

    // output verbose information
    if (settings.verbose || settings.info)
        printf("%s\n\nlaptop lid %s\n", renderUserInfo(outputs).c_str(),
               monitors.laptopLidClosed ? "closed" : "open or not present");

    // current info is all output, we're done
    if (settings.info)
        return EXIT_SUCCESS;

    // determine desired state
    orderOutputs(outputs, settings.order);
    const shared_ptr<Output> primary = activateOutputs(outputs, settings.primary, monitors);

    // arrange mirrored or left to right
    if (settings.mirror)
        mirrorOutputs(outputs);
    else
        ltrOutputs(outputs);

    // determine DPI from the primary
    string dpiExplaination;
    long dpi = calculateDpi(primary, dpiExplaination);
    if (settings.verbose)
        printf("\n\n%s\n", dpiExplaination.c_str());

    // render desired commands
    const string xrandrCmd = renderXrandrCmd(outputs, primary, dpi);
    const string xrdbCmd = renderXrdbCmd(dpi);
    if (settings.verbose || settings.dryRun)
        printf("\n%s\n\n%s\n", xrandrCmd.c_str(), xrdbCmd.c_str());

    // execute
    if (!settings.dryRun) {
        // xrandr
        int rc = system(xrandrCmd.c_str());
        if (rc != 0)
            return rc;

        // xrdb
        rc = system(xrdbCmd.c_str());
        if (rc != 0)
            return rc;

        // TODO: refactor
        // load the appropriate "left_ptr" cursor with the updated size and set it to the root window
        Display *dpy = XOpenDisplay(nullptr);
        int screen = DefaultScreen(dpy);
        Window root = RootWindow(dpy, screen);
        Cursor cursor = XcursorLibraryLoadCursor(dpy, "left_ptr");
        XDefineCursor(dpy, root, cursor);
        XFreeCursor(dpy, cursor);
        XCloseDisplay(dpy);
    }
    return EXIT_SUCCESS;
}
