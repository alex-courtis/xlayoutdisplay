#include "xrandrrutil.h"
#include "xrdbutil.h"
#include "xutil.h"
#include "calculations.h"
#include "Settings.h"

using namespace std;

const int layout(const Settings &settings) {

    // discover monitors
    const Monitors monitors = Monitors();

    // discover outputs
    const list<shared_ptr<Output>> currentOutputs = discoverOutputs();
    if (currentOutputs.empty())
        throw runtime_error("no outputs found");

    // output verbose information
    if (!settings.quiet || settings.info)
        printf("%s\n\nlaptop lid %s\n", renderUserInfo(currentOutputs).c_str(),
               monitors.laptopLidClosed ? "closed" : "open or not present");

    // current info is all output, we're done
    if (settings.info)
        return EXIT_SUCCESS;

    // order the outputs if the user wishes
    const list<shared_ptr<Output>> outputs = orderOutputs(currentOutputs, settings.order);

    // activate ouputs and determine primary
    const shared_ptr<Output> primary = activateOutputs(outputs, settings.primary, monitors);

    // arrange mirrored or left to right
    if (settings.mirror)
        mirrorOutputs(outputs);
    else
        ltrOutputs(outputs);

    // determine DPI from the primary
    string dpiExplaination;
    const long dpi = calculateDpi(primary, &dpiExplaination);
    if (!settings.quiet)
        printf("\n\n%s\n", dpiExplaination.c_str());

    // render desired commands
    const string xrandrCmd = renderXrandrCmd(outputs, primary, dpi);
    const string xrdbCmd = renderXrdbCmd(dpi);
    if (!settings.quiet || settings.noop)
        printf("\n%s\n\n%s\n", xrandrCmd.c_str(), xrdbCmd.c_str());

    // execute
    if (!settings.noop) {
        // xrandr
        int rc = system(xrandrCmd.c_str());
        if (rc != 0)
            return rc;

        // xrdb
        rc = system(xrdbCmd.c_str());
        if (rc != 0)
            return rc;

        // update root window's cursor
        resetRootCursor();
    }
    return EXIT_SUCCESS;
}
