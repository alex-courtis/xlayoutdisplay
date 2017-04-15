#include "xLayoutDisplays.h"

#include "Settings.h"
#include "Laptop.h"

#include "util.h"
#include "layout.h"
#include "xrandrrutil.h"

using namespace std;

// TODO: refactor this as a throw from cpp, with c functions returning zeros instead of FAIL
#define FAIL(...) { fprintf(stderr, "FAIL: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

// build a list of Displ based on the current and possible state of the world
const list <DisplP> discoverDispls() {
    list <DisplP> displs;

    // open up X information
    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) FAIL("Failed to open display defined by DISPLAY environment variable");
    int screen = DefaultScreen(dpy);
    if (screen >= ScreenCount(dpy)) FAIL("Invalid screen number %d (display has %d)\n", screen, ScreenCount(dpy));
    Window rootWindow = RootWindow(dpy, screen);
    XRRScreenResources *screenResources = XRRGetScreenResources(dpy, rootWindow);

    // iterate outputs
    for (int i = 0; i < screenResources->noutput - 1; i++) {
        Displ::State state;
        list <ModeP> modes;
        ModeP currentMode, preferredMode, optimalMode;
        PosP currentPos;

        // current state
        const XRROutputInfo *outputInfo = XRRGetOutputInfo(dpy, screenResources, screenResources->outputs[i]);
        const char *name = outputInfo->name;
        RRMode rrMode = 0;
        if (outputInfo->crtc != 0) {
            // active displays have CRTC info
            state = Displ::active;

            // current position and mode
            XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(dpy, screenResources, outputInfo->crtc);
            currentPos = make_shared<Pos>(crtcInfo->x, crtcInfo->y);
            rrMode = crtcInfo->mode;
            currentMode = shared_ptr<Mode>(Mode::fromXRR(rrMode, screenResources));

            if (outputInfo->nmode == 0) {
                // display is active but has been disconnected
                state = Displ::disconnected;
            }
        } else if (outputInfo->nmode != 0) {
            // inactive connected displays have modes available
            state = Displ::connected;
        } else {
            state = Displ::disconnected;
        }

        // add available modes
        for (int j = 0; j < outputInfo->nmode; j++) {
            if (state == Displ::disconnected) FAIL("apparently disconnected display has modes available");

            // add to modes
            const auto mode = shared_ptr<Mode>(Mode::fromXRR(outputInfo->modes[j], screenResources));
            modes.push_back(mode);

            // (optional) preferred mode based on outputInfo->modes indexed by 1
            if (outputInfo->npreferred == j + 1)
                preferredMode = mode;

            // replace currentMode with the one from the list
            if (mode->rrMode == rrMode)
                currentMode = mode;
        }

        // highest res/refresh is optimal mode
        if (!modes.empty()) {
            modes.sort(sortSharedPtr<Mode>);
            modes.reverse();
            optimalMode = *modes.begin();
        }

        // add the displ
        displs.push_back(make_shared<Displ>(name, state, modes, currentMode, preferredMode, optimalMode, currentPos));
    }
    return displs;
}

int run(int argc, char **argv) {
    try {
        Settings *settings = Settings::instance();
        
        // load persistent settings
        settings->loadUserSettings();

        // override with CLI settings
        settings->loadCliSettings(argc, argv);

        // discover current state
        list <DisplP> displs = discoverDispls();
        const bool lidClose = Laptop::instance()->isLidClosed();
        
        // output verbose information
        if (settings->verbose || settings->info) {
            printf("%s\n\nlid %s\n", renderUserInfo(displs).c_str(), lidClose ? "closed" : "open or not present");
        }

        // current info is all output, we're done
        if (settings->info)
            return EXIT_SUCCESS;

        // determine desired state
        orderDispls(displs, settings->order);
        activateDispls(displs, lidClose, settings->primary);

        // arrange left to right
        ltrDispls(displs);

        // render desired state for xrandr
        const string xrandr = renderCmd(displs);
        if (settings->verbose) {
            printf("\n%s\n", xrandr.c_str());
        }

        if (settings->dryRun) {
            if (!settings->verbose) {
                // still print xrandr command for quiet dry run
                printf("%s\n", xrandr.c_str());
            }
            return EXIT_SUCCESS;
        }

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
