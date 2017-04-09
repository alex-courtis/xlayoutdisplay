#include "xLayoutDisplays.h"

#include "laptop.h"
#include "layout.h"
#include "xrandrrutil.h"

#include <sstream>
#include <cstring>

#include <getopt.h>

using namespace std;

// TODO: refactor this as a throw from cpp, with c functions returning zeros instead of FAIL
#define FAIL(...) { fprintf(stderr, "FAIL: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

#define USAGE "Usage: %s [-h] [-i] [-n] [-o order] [-p primary] [-q]\n"

#define EMBEDDED_DISPLAY_PREFIX "eDP"

bool OPT_HELP = false;
bool OPT_INFO = false;
bool OPT_DRY_RUN = false;
list <string> OPT_ORDER;
string OPT_PRIMARY;
bool OPT_VERBOSE = true;

// sorting function for shared pointers... this must be in STL somewhere...
template<typename T>
bool sortSharedPtr(const shared_ptr<T> &l, const shared_ptr<T> &r) {
    return *l < *r;
}

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

// print info about all displs
void printDispls(const list <DisplP> &displs) {
    char current, preferred, optimal;
    for (const auto displ : displs) {
        printf("%s ", displ->name.c_str());
        switch (displ->state) {
            case Displ::active:
                printf("active");
                break;
            case Displ::connected:
                printf("connected");
                break;
            case Displ::disconnected:
                printf("disconnected");
                break;
        }
        if (displ->currentMode && displ->currentPos) {
            printf(" %ux%u%+d%+d %uHz", displ->currentMode->width, displ->currentMode->height, displ->currentPos->x,
                   displ->currentPos->y, displ->currentMode->refresh);
        }
        printf("\n");
        for (const auto mode : displ->modes) {
            current = mode == displ->currentMode ? '*' : ' ';
            preferred = mode == displ->preferredMode ? '+' : ' ';
            optimal = mode == displ->optimalMode ? '!' : ' ';
            printf("%c%c%c%ux%u %uHz\n", current, preferred, optimal, mode->width, mode->height, mode->refresh);
        }
    }
    printf("*current +preferred !optimal\n");
}

// display help and exit with success
void help(char *progname) {
    printf(""
                   "Detects and arranges displays in a left to right manner.\n"
                   "Invokes xrandr to perform arrangement.\n"
                   "Highest resolution and refresh are used for each display.\n"
                   "Displays starting with \"%s\" are disabled if the laptop lid is closed as per /proc/acpi/button/lid/.*/state\n"
                   "Displays are ordered via Xrandr default.\n"
                   "The first display will be primary unless -p specified.\n"
                   "\n", EMBEDDED_DISPLAY_PREFIX
    );
    printf(USAGE, progname);
    printf(""
                   "  -h  display this help text and exit\n"
                   "  -i  display information about current displays and exit\n"
                   "  -m  *mirror displays using the lowest common resolution (not ready yet, havering about panning)\n"
                   "  -n  perform a trial run and exit\n"
                   "  -o  order of displays, space/comma delimited\n"
                   "  -p  primary display\n"
                   "  -q  suppress output\n"
                   "  -v  *arrange displays in a top to bottom manner (is this needed?)\n"
    );
    printf("\n"
                   "e.g.: %s -o DP-0,HDMI-0 -p HDMI-0\n"
                   "  arranges DP-0 left, HDMI-0 right, with any remaining displays further right, with HDMI-0 as primary\n"
                   "", progname
    );
    exit(EXIT_SUCCESS);
}

// display usage and help hint then exit with failure
void usage(char *progname) {
    fprintf(stderr, USAGE, progname);
    fprintf(stderr, "Try '%s -h' for more information.\n", progname);
    exit(EXIT_FAILURE);
}

int run(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "hino:p:q")) != -1) {
        switch (opt) {
            case 'h':
                OPT_HELP = true;
                break;
            case 'i':
                OPT_INFO = true;
                break;
            case 'n':
                OPT_DRY_RUN = true;
                break;
            case 'o':
                for (char *token = strtok(optarg, " ,"); token != NULL; token = strtok(NULL, " ,"))
                    OPT_ORDER.push_back(string(token));
                break;
            case 'p':
                OPT_PRIMARY = optarg;
                break;
            case 'q':
                OPT_VERBOSE = false;
                break;
            default:
                usage(basename(argv[0]));
        }
    }
    if (argc > optind)
        usage(basename(argv[0]));
    if (OPT_HELP)
        help(basename(argv[0]));

    // discover current state
    list <DisplP> displs = discoverDispls();
    const bool lidClose = lidClosed();
    if (OPT_VERBOSE || OPT_INFO) {
        printDispls(displs);
        printf("\nlid %s\n", lidClose ? "closed" : "open or not present");
    }

    // current info is all output, we're done
    if (OPT_INFO)
        return EXIT_SUCCESS;

    // determine desired state
    orderDispls(displs, OPT_ORDER);
    activateDispls(displs, lidClose, OPT_PRIMARY, EMBEDDED_DISPLAY_PREFIX);

    // arrange left to right
    ltrDispls(displs);

    // render desired state for xrandr
    const string xrandr = renderCmd(displs);
    if (OPT_VERBOSE) {
        printf("\n%s\n", xrandr.c_str());
    }

    if (OPT_DRY_RUN) {
        if (!OPT_VERBOSE) {
            // print xrandr command for quiet dry run
            printf("%s\n", xrandr.c_str());
        }
        return EXIT_SUCCESS;
    }

    // invoke xrandr
    return system(xrandr.c_str());
}
