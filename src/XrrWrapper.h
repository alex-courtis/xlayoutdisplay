#ifndef XLAYOUTDISPLAYS_XRRWRAPPER_H
#define XLAYOUTDISPLAYS_XRRWRAPPER_H

#include <X11/extensions/Xrandr.h>

class XrrWrapper {
public:
    virtual ~XrrWrapper() {};

    virtual Display *xOpenDisplay(_Xconst char *name) const = 0;

    virtual int defaultScreen(Display *dpy) const = 0;

    virtual int screenCount(Display *dpy) const = 0;

    virtual Window rootWindow(Display *dpy, int scr) const = 0;

    virtual XRRScreenResources *xrrGetScreenResources(Display *dpy, Window window) const = 0;

    virtual XRROutputInfo *xrrGetOutputInfo(Display *dpy, XRRScreenResources *resources, RROutput output) const = 0;

    virtual XRRCrtcInfo *xrrGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc) const = 0;
};

class XrrWrapperImpl : public XrrWrapper {
public:
    Display *xOpenDisplay(_Xconst char *name) const {
        return XOpenDisplay(name);
    }

    int defaultScreen(Display *dpy) const {
        return DefaultScreen(dpy);
    }

    int screenCount(Display *dpy) const {
        return ScreenCount(dpy);
    }

    Window rootWindow(Display *dpy, int scr) const {
        return RootWindow(dpy, scr);
    }

    XRRScreenResources *xrrGetScreenResources(Display *dpy, Window window) const {
        return XRRGetScreenResources(dpy, window);
    }

    XRROutputInfo *xrrGetOutputInfo(Display *dpy, XRRScreenResources *resources, RROutput output) const {
        return XRRGetOutputInfo(dpy, resources, output);
    }

    XRRCrtcInfo *xrrGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc) const {
        return XRRGetCrtcInfo(dpy, resources, crtc);
    }
};

#endif //XLAYOUTDISPLAYS_XRRWRAPPER_H
