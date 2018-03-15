#include "layout.h"

#include "xrandrrutil.h"
#include "xrdbutil.h"
#include "xutil.h"
#include "calculations.h"
#include <iostream>

using namespace std;

const int layout(const Settings &settings) {

    // discover monitors
    const Monitors monitors = Monitors();

    // discover outputs
    const list<shared_ptr<Output>> currentOutputs = discoverOutputs();
    if (currentOutputs.empty()) {
        throw runtime_error("no outputs found");
    }

    // output verbose information
    if (!settings.quiet || settings.info) {
        cout << renderUserInfo(currentOutputs) << "\n\n";
        cout << "laptop lid ";
        if (monitors.laptopLidClosed) {
            cout << "closed";
        } else {
            cout << "open or not present";
        }
        cout << "\n";
    }

    // current info is all output, we're done
    if (settings.info) {
        return EXIT_SUCCESS;
    }

    // order the outputs if the user wishes
    const list<shared_ptr<Output>> outputs = orderOutputs(currentOutputs, settings.order);

    // activate ouputs and determine primary
    const shared_ptr<Output> primary = activateOutputs(outputs, settings.primary, monitors);

    // arrange mirrored or left to right
    if (settings.mirror) {
        mirrorOutputs(outputs);
    } else {
        ltrOutputs(outputs);
    }

    // determine DPI from the primary
    string dpiExplaination;
    const long dpi = calculateDpi(primary, &dpiExplaination);
    if (!settings.quiet) {
        cout << "\n" << dpiExplaination << "\n";
    }

    // render desired commands
    const string xrandrCmd = renderXrandrCmd(outputs, primary, dpi);
    const string xrdbCmd = renderXrdbCmd(dpi);
    if (!settings.quiet || settings.noop) {
        cout << "\n" << xrandrCmd << "\n\n" << xrdbCmd << "\n";
    }

    // execute
    if (!settings.noop) {
        // xrandr
        int rc = system(xrandrCmd.c_str());
        if (rc != 0) {
            return rc;
        }

        // xrdb
        rc = system(xrdbCmd.c_str());
        if (rc != 0) {
            return rc;
        }

        // update root window's cursor
        resetRootCursor();
    }
    return EXIT_SUCCESS;
}
