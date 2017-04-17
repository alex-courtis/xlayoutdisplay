#ifndef XLAYOUTDISPLAYS_XRRWRAPPER_H
#define XLAYOUTDISPLAYS_XRRWRAPPER_H

#include <X11/extensions/Xrandr.h>

class XRRWrapper {
public:
    virtual ~XRRWrapper() {};

    virtual Display *xOpenDisplay(_Xconst char *name) = 0;

    virtual int defaultScreen(Display *dpy) = 0;

    virtual int screenCount(Display *dpy) = 0;

    virtual Window rootWindow(Display *dpy, int scr) = 0;

    virtual XRRScreenResources *xrrGetScreenResources(Display *dpy, Window window) = 0;

    virtual XRROutputInfo *xrrGetOutputInfo(Display *dpy, XRRScreenResources *resources, RROutput output) = 0;

    virtual XRRCrtcInfo *xrrGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc) = 0;
};

class XRRWrapperImpl : public XRRWrapper {
public:
    Display *xOpenDisplay(_Xconst char *name) {
        return XOpenDisplay(name);
    }

    int defaultScreen(Display *dpy) {
        return DefaultScreen(dpy);
    }

    int screenCount(Display *dpy) {
        return ScreenCount(dpy);
    }

    Window rootWindow(Display *dpy, int scr) {
        return RootWindow(dpy, scr);
    }

    XRRScreenResources *xrrGetScreenResources(Display *dpy, Window window) {
        return XRRGetScreenResources(dpy, window);
    }

    XRROutputInfo *xrrGetOutputInfo(Display *dpy, XRRScreenResources *resources, RROutput output) {
        return XRRGetOutputInfo(dpy, resources, output);
    }

    XRRCrtcInfo *xrrGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc) {
        return XRRGetCrtcInfo(dpy, resources, crtc);
    }
};

#endif //XLAYOUTDISPLAYS_XRRWRAPPER_H
