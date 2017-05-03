#ifndef XLAYOUTDISPLAYS_XRANDRUTIL_H
#define XLAYOUTDISPLAYS_XRANDRUTIL_H

#include "Displ.h"

class XrrWrapper {
public:
    virtual ~XrrWrapper() {};

    virtual Display *xOpenDisplay(_Xconst char *name) const {
        return XOpenDisplay(name);
    }

    virtual int defaultScreen(Display *dpy) const {
        return DefaultScreen(dpy);
    }

    virtual int screenCount(Display *dpy) const {
        return ScreenCount(dpy);
    }

    virtual Window rootWindow(Display *dpy, int scr) const {
        return RootWindow(dpy, scr);
    }

    virtual XRRScreenResources *xrrGetScreenResources(Display *dpy, Window window) const {
        return XRRGetScreenResources(dpy, window);
    }

    virtual XRROutputInfo *xrrGetOutputInfo(Display *dpy, XRRScreenResources *resources, RROutput output) const {
        return XRRGetOutputInfo(dpy, resources, output);
    }

    virtual XRRCrtcInfo *xrrGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc) const {
        return XRRGetCrtcInfo(dpy, resources, crtc);
    }
};

// v refresh frequency in even Hz, zero if modeInfo is NULL
const unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo);

// render xrandr cmd to layout displays
// will activate only if desiredActive, desiredMode, desiredPos are set
// desiredPrimary is only set if activated
// DPI will be set to that of the desiredPrimary which specifies it, otherwise 96
const std::string renderCmd(const std::list<DisplP> &displs);

// render a user readable string explaining the current state of displs
const std::string renderUserInfo(const std::list<DisplP> &displs);

// build a list of Displ based on the current and possible state of the world; override xrrWrapper only for testing
const std::list<DisplP> discoverDispls(const XrrWrapper *xrrWrapper = new XrrWrapper());

#endif //XLAYOUTDISPLAYS_XRANDRUTIL_H
