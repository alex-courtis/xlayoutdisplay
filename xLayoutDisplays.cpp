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

#define FAIL(...) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }

class Mode {
public:
    Mode(const unsigned int &width, const unsigned int &height, const unsigned int &refresh) :
            width(width), height(height), refresh(refresh) {
    }

    Mode(const Mode &mode) :
            width(0), height(0), refresh(0) {
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
        active, connected, disconnected
    };

    Displ(const char *name, const State &state, const list <ModeP> &modes, const ModeP &currentMode,
          const ModeP preferredMode,
          const ModeP desiredMode, const PosP currentPos) :
            name(name), state(state), modes(modes), currentMode(currentMode), preferredMode(preferredMode),
            desiredMode(desiredMode), currentPos(currentPos) {
        switch (state) {
            case active:
                if (!currentMode) FAIL("active Displ has no currentMode")
                if (!currentPos) FAIL("active Displ has no currentPos")
                if (!preferredMode) FAIL("active Displ has no preferredMode")
                if (!desiredMode) FAIL("active Displ has no desiredMode")
                break;
            case connected:
                if (!desiredMode) FAIL("connected Displ has no desiredMode")
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
    // todo: make this changeable
    const ModeP desiredMode;
    const PosP currentPos;
};

typedef shared_ptr<Displ> DisplP;

// sorting function for shared pointers... this must be in STL somewhere...
template<typename T>
bool sortSharedPtr(const shared_ptr<T> &l, const shared_ptr<T> &r) {
    return *l < *r;
}

// find the mode info for the id passed, in resources
const XRRModeInfo *modeInfoFromId(const RRMode id, const XRRScreenResources *resources) {
    for (int i = 0; i < resources->nmode; i++) {
        if (id == resources->modes[i].id) {
            return &(resources->modes[i]);
        }
    }
    FAIL("Can't find mode %lu from screen resources", id);
    return NULL;
}

// v refresh frequency in Hz - stolen from xrandr.c
const unsigned int refreshFromModeInfo(const XRRModeInfo *modeInfo) {
    double rate;
    double vTotal = modeInfo->vTotal;

    if (modeInfo->modeFlags & RR_DoubleScan) {
        /* doublescan doubles the number of lines */
        vTotal *= 2;
    }

    if (modeInfo->modeFlags & RR_Interlace) {
        /* interlace splits the frame into two fields */
        /* the field rate is what is typically reported by monitors */
        vTotal /= 2;
    }

    if (modeInfo->hTotal && vTotal)
        rate = ((double) modeInfo->dotClock / (modeInfo->hTotal * vTotal));
    else
        rate = 0;

    // round up, as xrandr uses the greatest rate less than passed
    return (unsigned int) (rate + 0.5);
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
        char *name = NULL;
        Displ::State state;
        list <ModeP> modes;
        ModeP currentMode, preferredMode, desiredMode;
        PosP currentPos;

        // basic info
        XRROutputInfo *outputInfo = XRRGetOutputInfo(dpy, screenResources, screenResources->outputs[i]);
        name = outputInfo->name;
        if (outputInfo->crtc != 0) {
            // active displays have CRTC info
            state = Displ::active;
        } else if (outputInfo->nmode != 0) {
            // inactive connected displays have modes available
            state = Displ::connected;
        } else {
            state = Displ::disconnected;
        }

        // current display mode and position for active inputs
        RRMode rrMode = 0;
        if (state == Displ::active) {
            XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(dpy, screenResources, outputInfo->crtc);
            rrMode = crtcInfo->mode;
            currentPos = make_shared<Pos>(crtcInfo->x, crtcInfo->y);
        }

        // iterate available modes
        for (int j = 0; j < outputInfo->nmode; j++) {
            unsigned int width, height, refresh;

            // gather mode info
            const XRRModeInfo *modeInfo = modeInfoFromId(outputInfo->modes[j], screenResources);
            width = modeInfo->width;
            height = modeInfo->height;
            refresh = refreshFromModeInfo(modeInfo);

            // add to modes
            auto mode = make_shared<Mode>(width, height, refresh);
            modes.push_back(mode);

            // (optional) preferred mode based on outputInfo->modes indexed by 1
            if (outputInfo->npreferred == j + 1) {
                preferredMode = mode;
            }

            // is this mode being used?
            if (modeInfo->id == rrMode) {
                currentMode = mode;
            }
        }

        // default best mode to desired
        if (!modes.empty()) {
            modes.sort(sortSharedPtr<Mode>);
            desiredMode = *modes.begin();
        }

        // add the displ
        displs.push_back(make_shared<Displ>(name, state, modes, currentMode, preferredMode, desiredMode, currentPos));
    }
    return displs;
}

// print info about all displs
void printDispls(const list <DisplP> &displs) {
    for (auto displ : displs) {
        printf("%s ", displ->name);
        switch (displ->state) {
            case Displ::active:
                printf("active %ux%u%+d%+d %uHz\n", displ->currentMode->width, displ->currentMode->height,
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
            char current = mode == displ->currentMode ? '*' : ' ';
            char preferred = mode == displ->preferredMode ? '+' : ' ';
            char desired = mode == displ->desiredMode ? '!' : ' ';
            printf("%c%c%c%ux%u %uHz\n", current, preferred, desired, mode->width, mode->height, mode->refresh);
        }
    }
    printf("\n*current +preferred !desired\n");
}

// layout displays from left to right
void leftToRight(const list <DisplP> &displs) {
    int xpos = 0;
    for (auto displ : displs) {
        switch (displ->state) {
            case Displ::active:
            case Displ::connected:
                xpos += displ->desiredMode->width;
                // TODO: implement and update desired position
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
                ss << " --mode " << displ->desiredMode->width << "x" << displ->desiredMode->height;
                ss << " --rate " << displ->desiredMode->refresh;
                // todo: use desired position instead
                ss << " --pos ";
                if (displ->currentPos) {
                    ss << displ->currentPos->x << "x" << displ->currentPos->y;
                } else {
                    ss << "0x0";
                }
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

    leftToRight(displs);

    printXrandr(displs);

    return EXIT_SUCCESS;
}
