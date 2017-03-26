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
#include <cstring>
#include <fstream>

#include <dirent.h>
#include <getopt.h>

#include <X11/extensions/Xrandr.h>

using namespace std;

#define FAIL(...) { fprintf(stderr, "FAIL: "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

bool OPT_DRY_RUN = false;
list<string> OPT_ORDER;
char *OPT_PRIMARY = NULL;
bool OPT_VERBOSE = true;

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
        active, connected, disconnected
    };

    Displ(const char *name, const State &state, const list <ModeP> &modes, const ModeP &currentMode,
          const ModeP preferredMode,
          const ModeP optimalMode, const PosP currentPos) :
            name(name), state(state), modes(modes), currentMode(currentMode), preferredMode(preferredMode),
            optimalMode(optimalMode), currentPos(currentPos) {
        if (name == NULL) FAIL("Displ has no name")
        switch (state) {
            case active:
                if (!currentMode) FAIL("active Displ %s has no currentMode", name)
                if (!currentPos) FAIL("active Displ %s has no currentPos", name)
                if (!optimalMode) FAIL("active Displ %s has no optimalMode", name)
                if (modes.empty()) FAIL("active Displ %s has no modes", name)
                break;
            case connected:
                if (!optimalMode) FAIL("connected Displ %s has no optimalMode", name)
                if (modes.empty()) FAIL("connected Displ %s has no modes", name)
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

    bool desiredPrimary = false;
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
            currentMode = modeFromId(rrMode, screenResources);

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
            auto mode = modeFromId(outputInfo->modes[j], screenResources);
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
        for (auto mode : displ->modes) {
            current = mode == displ->currentMode ? '*' : ' ';
            preferred = mode == displ->preferredMode ? '+' : ' ';
            optimal = mode == displ->optimalMode ? '!' : ' ';
            printf("%c%c%c%ux%u %uHz\n", current, preferred, optimal, mode->width, mode->height, mode->refresh);
        }
    }
    printf("*current +preferred !optimal\n");
}

// arrange the displays
void arrangeDispls(const list <DisplP> &displs, const bool &lidClosed) {
    static const char *EMBEDDED_DISPLAY_PREFIX = "edp";

    int xpos = 0;
    int ypos = 0;
    bool primarySet = false;
    for (auto displ : displs) {

        if (lidClosed && strncasecmp(EMBEDDED_DISPLAY_PREFIX, displ->name, strlen(EMBEDDED_DISPLAY_PREFIX)) == 0) {
            // don't use any embedded displays if the lid is closed
            continue;
        }

        if (displ->state == Displ::active || displ->state == Displ::connected) {

            if (!primarySet) {
                if (OPT_PRIMARY) {
                    if (strcasecmp(displ->name, OPT_PRIMARY) == 0) {
                        // user selected primary
                        displ->desiredPrimary = true;
                        primarySet = true;
                    }
                } else {
                    // default first to primary
                    displ->desiredPrimary = true;
                    primarySet = true;
                }
            }

            // set the desired mode to optimal
            displ->desiredMode = displ->optimalMode;

            // position the screen
            displ->desiredPos = make_shared<Pos>(xpos, ypos);

            // next position
            xpos += displ->desiredMode->width;
        }
    }

    if (!primarySet && OPT_PRIMARY) {
        FAIL("Invalid primary monitor \"%s\"", OPT_PRIMARY);
    }
}

// print xrandr cmd for any displays with desired mode and position
const string renderXrandr(const list <DisplP> &displs) {
    stringstream ss;
    ss << "xrandr";
    for (auto displ : displs) {
        ss << " \\\n";
        ss << " --output " << displ->name;
        if (displ->desiredMode && displ->desiredPos) {
            ss << " --mode " << displ->desiredMode->width << "x" << displ->desiredMode->height;
            ss << " --rate " << displ->desiredMode->refresh;
            ss << " --pos ";
            ss << displ->desiredPos->x << "x" << displ->desiredPos->y;
            if (displ->desiredPrimary) {
                ss << " --primary";
            }
        } else {
            ss << " --off";
        }
    }
    return ss.str();
}

// return true if we have a "closed" status in the file like /proc/acpi/button/lid/LID0/state
bool isLidClosed() {
    bool lidClosed = false;

    static const char *LID_DIR = "/proc/acpi/button/lid";

    // find the lid state directory
    DIR *dir = opendir(LID_DIR);
    if (dir) {
        struct dirent *dirent;
        while ((dirent = readdir(dir)) != NULL) {
            if (dirent->d_type == DT_DIR && strcmp(dirent->d_name, ".") != 0 && strcmp(dirent->d_name, "..") != 0) {

                // read the lid state file
                stringstream lidFileName;
                lidFileName << LID_DIR << "/" << dirent->d_name << "/" << "state";
                ifstream lidFile;
                lidFile.open(lidFileName.str().c_str(), ios::in);
                if (lidFile.is_open()) {
                    string line;
                    if (getline(lidFile, line)) {
                        if (line.find("closed") != string::npos) {
                            lidClosed = true;
                        }
                    }
                    lidFile.close();
                }

                // drivers/acpi/button.c acpi_button_add_fs seems to indicate there will be only one
                break;
            }
        }
        closedir(dir);
    }
    return lidClosed;
}

void usage(char *prog) {
    fprintf(stderr, "Usage: %s [-n] [-o comma delimited display order] [-p primary] [-q]\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "no:p:q")) != -1) {
        switch (opt) {
            case 'n':
                OPT_DRY_RUN = true;
                break;
            case 'o':
                for (char *token = strtok(optarg, ","); token != NULL; token = strtok(NULL, ",")) {
                    OPT_ORDER.push_back(string(token));
                }
                break;
            case 'p':
                OPT_PRIMARY = optarg;
                break;
            case 'q':
                OPT_VERBOSE = false;
                break;
            default:
                usage(argv[0]);
        }
    }
    if (argc > optind) usage(argv[0]);

    // discover current state
    const list <DisplP> displs = discoverDispls();
    const bool lidClosed = isLidClosed();
    if (OPT_VERBOSE) {
        printDispls(displs);
        printf("\nlid %s\n", lidClosed ? "closed" : "open or not present");
    }

    // determine desired state
    arrangeDispls(displs, lidClosed);

    // render desired state for xrandr
    const string xrandr = renderXrandr(displs);
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