#include "xrandrrutil.h"
#include "util.h"

#include <sstream>
#include <string.h>

using namespace std;

// stolen from xrandr.c; assuming this mostly works
const unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo) {
    double rate;
    double vTotal = modeInfo.vTotal;

    /* doublescan doubles the number of lines */
    if (modeInfo.modeFlags & RR_DoubleScan)
        vTotal *= 2;

    /* interlace splits the frame into two fields */
    /* the field rate is what is typically reported by monitors */
    if (modeInfo.modeFlags & RR_Interlace)
        vTotal /= 2;

    if (modeInfo.hTotal && vTotal)
        rate = ((double) modeInfo.dotClock / (modeInfo.hTotal * vTotal));
    else
        rate = 0;

    // round up, as xrandr uses the greatest rate less than passed
    return (unsigned int) (rate + 0.5);
}

const string renderCmd(const list <DisplP> &displs) {
    stringstream ss;
    ss << "xrandr";
    for (const auto displ : displs) {
        ss << " \\\n";
        ss << " --output " << displ->name;
        if (displ->desiredActive && displ->desiredMode && displ->desiredPos) {
            ss << " --mode " << displ->desiredMode->width << "x" << displ->desiredMode->height;
            ss << " --rate " << displ->desiredMode->refresh;
            ss << " --pos ";
            ss << displ->desiredPos->x << "x" << displ->desiredPos->y;
            if (displ == Displ::desiredPrimary) {
                ss << " --primary";
            }
        } else {
            ss << " --off";
        }
    }
    return ss.str();
}

const string renderUserInfo(const list <DisplP> &displs) {
    stringstream ss;
    for (const auto displ : displs) {
        ss << displ->name;
        switch (displ->state) {
            case Displ::active:
                ss << " active";
                break;
            case Displ::connected:
                ss << " connected";
                break;
            case Displ::disconnected:
                ss << " disconnected";
                break;
        }
        if (displ->currentMode && displ->currentPos) {
            ss << ' ' << displ->currentMode->width << 'x' << displ->currentMode->height;
            ss << '+' << displ->currentPos->x << '+' << displ->currentPos->y;
            ss << ' ' << displ->currentMode->refresh << "Hz";
        }
        if (displ->edid) {
            ss << ' ' << displ->edid->maxCmHoriz() << "cmx" << displ->edid->maxCmVert() << "cm";
        }
        ss << endl;
        for (const auto mode : displ->modes) {
            ss << (mode == displ->currentMode ? '*' : ' ');
            ss << (mode == displ->preferredMode ? '+' : ' ');
            ss << (mode == displ->optimalMode ? '!' : ' ');
            ss << mode->width << 'x' << mode->height << ' ' << mode->refresh << "Hz";
            ss << endl;
        }
    }
    ss << "*current +preferred !optimal";
    return ss.str();
}

// TODO: refactor this as a throw from cpp, with c functions returning zeros instead of FAIL
#define FAIL(...) { fprintf(stderr, "FAIL: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

// build a list of Displ based on the current and possible state of the world
const list <DisplP> discoverDispls(XrrWrapper *xrrWrapper) {
    bool deleteWrapper = false;
    if (xrrWrapper == NULL) {
        xrrWrapper = new XrrWrapperImpl();
        deleteWrapper = true;
    }

    list <DisplP> displs;

    // open up X information
    Display *dpy = xrrWrapper->xOpenDisplay(NULL);
    if (dpy == NULL)
        throw runtime_error("Failed to open display defined by DISPLAY environment variable");
    int screen = xrrWrapper->defaultScreen(dpy);
    if (screen >= xrrWrapper->screenCount(dpy))
        throw runtime_error("Invalid screen number " + to_string(screen) + " (display has " + to_string(xrrWrapper->screenCount(dpy)) + ")");
    Window rootWindow = xrrWrapper->rootWindow(dpy, screen);
    XRRScreenResources *screenResources = xrrWrapper->xrrGetScreenResources(dpy, rootWindow);

    // iterate outputs
    for (int i = 0; i < screenResources->noutput - 1; i++) {
        Displ::State state;
        list <ModeP> modes;
        ModeP currentMode, preferredMode, optimalMode;
        PosP currentPos;
        EdidP edid;

        // current state
        const RROutput rrOutput = screenResources->outputs[i];
        const XRROutputInfo *outputInfo = xrrWrapper->xrrGetOutputInfo(dpy, screenResources, rrOutput);
        const char *name = outputInfo->name;
        RRMode rrMode = 0;
        if (outputInfo->crtc != 0) {
            // active displays have CRTC info
            state = Displ::active;

            // current position and mode
            XRRCrtcInfo *crtcInfo = xrrWrapper->xrrGetCrtcInfo(dpy, screenResources, outputInfo->crtc);
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

        // iterate all properties
        int nprop;
        Atom *atoms = XRRListOutputProperties(dpy, rrOutput, &nprop);
        for (int j = 0; j < nprop; j++) {
            Atom atom = atoms[j];
            char *atomName = XGetAtomName(dpy, atom);

            // drill down on Edid
            if (strcmp(atomName, RR_PROPERTY_RANDR_EDID) == 0) {

                // retrieve property specifics
                Atom actualType;
                int actualFormat;
                unsigned long nitems, bytesAfter;
                unsigned char *prop;
                XRRGetOutputProperty(dpy, rrOutput, atom,
                                     0, // offset
                                     100, //length - NFI, copied from xrandr.c
                                     false, // delete
                                     false, // pending
                                     AnyPropertyType, &actualType, &actualFormat, &nitems, &bytesAfter, &prop
                );

                // record Edid
                edid = make_shared<EdidImpl>(prop, nitems, name);
            }
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
        displs.push_back(make_shared<Displ>(name, state, modes, currentMode, preferredMode, optimalMode, currentPos, edid));
    }

    if (deleteWrapper)
        delete (xrrWrapper);

    return displs;
}