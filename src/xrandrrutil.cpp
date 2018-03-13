#include "xrandrrutil.h"

#include <sstream>
#include <cstring>
#include <cmath>

using namespace std;

// stolen from xrandr.c; assuming this works, as we're reliant on xrandr anyway
const unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo) {
    double rate;
    double vTotal = modeInfo.vTotal;

    if (modeInfo.modeFlags & RR_DoubleScan)
        /* doublescan doubles the number of lines */
        vTotal *= 2;

    if (modeInfo.modeFlags & RR_Interlace)
        /* interlace splits the frame into two fields */
        /* the field rate is what is typically reported by monitors */
        vTotal /= 2;

    if (modeInfo.hTotal && vTotal)
        rate = ((double) modeInfo.dotClock / ((double) modeInfo.hTotal * vTotal));
    else
        rate = 0;

    // round up
    return static_cast<const unsigned int>(round(rate));
}

const string renderXrandrCmd(const list<shared_ptr<Output>> &outputs, const shared_ptr<Output> &primary, const long &dpi) {
    stringstream ss;
    ss << "xrandr \\\n --dpi " << dpi;
    for (const auto &output : outputs) {
        ss << " \\\n --output " << output->name;
        if (output->desiredActive && output->desiredMode && output->desiredPos) {
            ss << " --mode " << output->desiredMode->width << "x" << output->desiredMode->height;
            ss << " --rate " << output->desiredMode->refresh;
            ss << " --pos ";
            ss << output->desiredPos->x << "x" << output->desiredPos->y;
            if (output == primary) {
                ss << " --primary";
            }
        } else {
            ss << " --off";
        }
    }
    return ss.str();
}

Mode *modeFromXRR(RRMode id, const XRRScreenResources *resources) {
    if (resources == nullptr)
        throw invalid_argument("cannot construct Mode: NULL XRRScreenResources");

    XRRModeInfo *modeInfo = nullptr;
    for (int i = 0; i < resources->nmode; i++) {
        if (id == resources->modes[i].id) {
            modeInfo = &(resources->modes[i]);
            break;
        }
    }

    if (modeInfo == nullptr)
        throw invalid_argument("cannot construct Mode: cannot retrieve RRMode '" + to_string(id) + "'");

    return new Mode(id, modeInfo->width, modeInfo->height, refreshFromModeInfo(*modeInfo));
}

// build a list of Output based on the current and possible state of the world
const list<shared_ptr<Output>> discoverOutputs() {
    list<shared_ptr<Output>> outputs;

    // get the display and root window
    Display *dpy = XOpenDisplay(nullptr);
    int screen = DefaultScreen(dpy);
    Window rootWindow = RootWindow(dpy, screen);

    // get RandR resources
    XRRScreenResources *screenResources = XRRGetScreenResources(dpy, rootWindow);

    // iterate outputs
    for (int i = 0; i < screenResources->noutput - 1; i++) {
        Output::State state;
        list<std::shared_ptr<const Mode>> modes;
        std::shared_ptr<Mode> currentMode, preferredMode;
        shared_ptr<Pos> currentPos;
        shared_ptr<Edid> edid;

        // current state
        const RROutput rrOutput = screenResources->outputs[i];
        const XRROutputInfo *outputInfo = XRRGetOutputInfo(dpy, screenResources, rrOutput);
        const char *name = outputInfo->name;
        RRMode rrMode = 0;
        if (outputInfo->crtc != 0) {
            // active outputs have CRTC info
            state = Output::active;

            // current position and mode
            XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(dpy, screenResources, outputInfo->crtc);
            currentPos = make_shared<Pos>(crtcInfo->x, crtcInfo->y);
            rrMode = crtcInfo->mode;
            currentMode = shared_ptr<Mode>(modeFromXRR(rrMode, screenResources));

            if (outputInfo->nmode == 0) {
                // output is active but has been disconnected
                state = Output::disconnected;
            }
        } else if (outputInfo->nmode != 0) {
            // inactive connected outputs have modes available
            state = Output::connected;
        } else {
            state = Output::disconnected;
        }

        // iterate all properties to find EDID; XRRQueryOutputProperty fails when queried with XInternAtom
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
                                     64, // length in CARD32 - EDID 2.0 max length is 256 bytes
                                     false, // delete
                                     false, // pending
                                     AnyPropertyType, &actualType, &actualFormat, &nitems, &bytesAfter, &prop
                );

                // record Edid
                edid = make_shared<Edid>(prop, nitems, name);
            }
        }

        // add available modes
        for (int j = 0; j < outputInfo->nmode; j++) {

            // add to modes
            const auto mode = shared_ptr<Mode>(modeFromXRR(outputInfo->modes[j], screenResources));
            modes.push_back(mode);

            // (optional) preferred mode based on outputInfo->modes indexed by 1
            if (outputInfo->npreferred == j + 1)
                preferredMode = mode;

            // replace currentMode with the one from the list
            if (mode->rrMode == rrMode)
                currentMode = mode;
        }

        // add the output
        outputs.push_back(make_shared<Output>(name, state, modes, currentMode, preferredMode, currentPos, edid));
    }

    return outputs;
}