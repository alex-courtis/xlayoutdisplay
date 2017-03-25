//============================================================================
// Name        : xLayoutDisplays.cpp
// Author      : Alexander Courtis
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <memory>
#include <list>
#include <sstream>

#include <X11/extensions/Xrandr.h>

using namespace std;

#define FAIL(...) { fprintf(stderr, "FAIL: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

class Mode {
public:
    Mode(const RRMode &rrMode, const unsigned int &width, const unsigned int &height, const unsigned int &refresh) :
            rrMode(rrMode), width(width), height(height), refresh(refresh) {
    }

    Mode(const Mode &mode) :
            rrMode(0), width(0), height(0), refresh(0) {
        FAIL("copy constructing Mode");
    }

    ~Mode() {
    }

    // order by width, refresh, height in descending order
    bool operator<(const Mode &o) {
        if (width == o.width)
            if (refresh == o.refresh)
                return height > o.height;
            else
                return refresh > o.refresh;
        else
            return width > o.width;
    }

    const RRMode rrMode;
    const unsigned int width;
    const unsigned int height;
    const unsigned int refresh;
};

typedef shared_ptr<Mode> ModeP;

class Pos {
public:
    Pos(const int &x, const int &y) :
            x(x), y(y) {
    }

    Pos(const Pos &pos) {
        FAIL("copy constructing Pos");
    }

    ~Pos() {
    }

    const int x = 0;
    const int y = 0;
};

typedef shared_ptr<Pos> PosP;

class Displ {
public:
    enum State {
        active, removed, connected, disconnected
    };

    Displ(const char *name, const State &state, const list <ModeP> &modes, const ModeP &currentMode,
          const ModeP preferredMode,
          const ModeP optimalMode, const PosP currentPos) :
            name(name), state(state), modes(modes), currentMode(currentMode), preferredMode(preferredMode),
            optimalMode(optimalMode), currentPos(currentPos) {
        switch (state) {
            case active:
                if (!currentMode) FAIL("active Displ has no currentMode")
                if (!currentPos) FAIL("active Displ has no currentPos")
                if (!optimalMode) FAIL("active Displ has no optimalMode")
                break;
            case removed:
                if (!currentMode) FAIL("removed Displ has no currentMode")
                if (!currentPos) FAIL("removed Displ has no currentPos")
                break;
            case connected:
                if (!optimalMode) FAIL("connected Displ has no optimalMode")
                break;
            default:
                break;
        }
    }

    Displ(const Displ &displ) :
            name(NULL), state(disconnected) {
        FAIL("copy constructing Displ");
    }

    ~Displ() {
    }

    const char *name;
    const State state;

    const list <ModeP> modes;
    const ModeP currentMode;
    const ModeP preferredMode;
    const ModeP optimalMode;
    ModeP desiredMode;

    const PosP currentPos;
    PosP desiredPos;
};

typedef shared_ptr<Displ> DisplP;

// sorting function for shared pointers... this must be in STL somewhere...
template<typename T>
bool sortSharedPtr(const shared_ptr<T> &l, const shared_ptr<T> &r) {
    return *l < *r;
}

// find the mode info for the id passed, in resources
const XRRModeInfo *modeInfoFromId(const RRMode id, const XRRScreenResources *resources) {
    for (int i = 0; i < resources->nmode; i++)
        if (id == resources->modes[i].id)
            return &(resources->modes[i]);
    FAIL("Can't find mode %lu from screen resources", id);
    return NULL;
}

// v refresh frequency in Hz - stolen from xrandr.c
const unsigned int refreshFromModeInfo(const XRRModeInfo *modeInfo) {
    double rate;
    double vTotal = modeInfo->vTotal;

    /* doublescan doubles the number of lines */
    if (modeInfo->modeFlags & RR_DoubleScan)
        vTotal *= 2;

    /* interlace splits the frame into two fields */
    /* the field rate is what is typically reported by monitors */
    if (modeInfo->modeFlags & RR_Interlace)
        vTotal /= 2;

    if (modeInfo->hTotal && vTotal)
        rate = ((double) modeInfo->dotClock / (modeInfo->hTotal * vTotal));
    else
        rate = 0;

    // round up, as xrandr uses the greatest rate less than passed
    return (unsigned int) (rate + 0.5);
}

// create a ModeP from an RRMode
const ModeP modeFromId(const RRMode id, const XRRScreenResources *resources) {
    const XRRModeInfo *modeInfo = modeInfoFromId(id, resources);
    return make_shared<Mode>(id, modeInfo->width, modeInfo->height, refreshFromModeInfo(modeInfo));
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

        // basic info
        const XRROutputInfo *outputInfo = XRRGetOutputInfo(dpy, screenResources, screenResources->outputs[i]);
        const char *name = outputInfo->name;
        if (outputInfo->crtc != 0)
            if (outputInfo->nmode != 0)
                // currently active displays have CRTC info and available modes
                state = Displ::active;
            else
                // previously active displays have CRTC info but no available modes
                state = Displ::removed;
        else if (outputInfo->nmode != 0)
            // inactive connected displays have modes available
            state = Displ::connected;
        else
            state = Displ::disconnected;

        // current position and mode for active and removed inputs
        RRMode rrMode = 0;
        if (state == Displ::active || state == Displ::removed) {
            XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(dpy, screenResources, outputInfo->crtc);
            currentPos = make_shared<Pos>(crtcInfo->x, crtcInfo->y);

            // note mode for active inputs
            if (state == Displ::active)
                rrMode = crtcInfo->mode;

            // add mode for removed inputs
            if (state == Displ::removed)
                currentMode = modeFromId(crtcInfo->mode, screenResources);
        }

        // iterate available modes
        for (int j = 0; j < outputInfo->nmode; j++) {
            if (state == Displ::removed || state == Displ::disconnected) FAIL(
                    "apparently removed or disconnected display has modes available");

            // add to modes
            auto mode = modeFromId(outputInfo->modes[j], screenResources);
            modes.push_back(mode);

            // (optional) preferred mode based on outputInfo->modes indexed by 1
            if (outputInfo->npreferred == j + 1)
                preferredMode = mode;

            // is this mode being used?
            if (mode->rrMode == rrMode)
                currentMode = mode;
        }

        // hightest res/refresh is optimal mode
        if (!modes.empty()) {
            modes.sort(sortSharedPtr<Mode>);
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
    for (auto displ : displs) {
        printf("%s ", displ->name);
        switch (displ->state) {
            case Displ::active:
                printf("active %ux%u%+d%+d %uHz\n", displ->currentMode->width, displ->currentMode->height,
                       displ->currentPos->x, displ->currentPos->y,
                       displ->currentMode->refresh);
                break;
            case Displ::removed:
                printf("removed %ux%u%+d%+d %uHz\n", displ->currentMode->width, displ->currentMode->height,
                       displ->currentPos->x, displ->currentPos->y,
                       displ->currentMode->refresh);
                break;
            case Displ::connected:
                printf("connected\n");
                break;
            case Displ::disconnected:
                printf("disconnected\n");
                break;
        }
        for (auto mode : displ->modes) {
            current = mode == displ->currentMode ? '*' : ' ';
            preferred = mode == displ->preferredMode ? '+' : ' ';
            optimal = mode == displ->optimalMode ? '!' : ' ';
            printf("%c%c%c%ux%u %uHz\n", current, preferred, optimal, mode->width, mode->height, mode->refresh);
        }
    }
    printf("\n*current +preferred !optimal\n");
}

// arrange the displays
void arrangeDispls(const list <DisplP> &displs) {
    int xpos = 0;
    int ypos = 0;
    for (auto displ : displs) {
        // set the desired mode
        displ->desiredMode = displ->optimalMode;

        // position the screen
        displ->desiredPos = make_shared<Pos>(xpos, ypos);

        // next position
        switch (displ->state) {
            case Displ::active:
            case Displ::connected:
                xpos += displ->desiredMode->width;
                break;
            default:
                break;
        }
    }
}

// print xrandr cmd
void printXrandr(const list <DisplP> &displs) {
    stringstream ss;
    ss << "\nxrandr";
    for (auto displ : displs) {
        ss << " --output " << displ->name;
        switch (displ->state) {
            case Displ::active:
            case Displ::connected:
                if (!displ->desiredMode) FAIL("desiredMode not set for active or connected display")
                if (!displ->desiredPos) FAIL("desiredPos not set for active or connected display")
                ss << " --mode " << displ->desiredMode->width << "x" << displ->desiredMode->height;
                ss << " --rate " << displ->desiredMode->refresh;
                ss << " --pos ";
                ss << displ->desiredPos->x << "x" << displ->desiredPos->y;
                break;
            default:
                ss << " --off";
                break;
        }
    }
    printf("%s\n", ss.str().c_str());
}

int main() {
    const list <DisplP> displs = discoverDispls();

    printDispls(displs);

    arrangeDispls(displs);

    printXrandr(displs);

    return EXIT_SUCCESS;
}
