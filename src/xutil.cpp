#include <X11/Xcursor/Xcursor.h>

void resetRootCursor() {
    Display *dpy = XOpenDisplay(nullptr);
    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);
    Cursor cursor = XcursorLibraryLoadCursor(dpy, "left_ptr");
    XDefineCursor(dpy, root, cursor);
    XFreeCursor(dpy, cursor);
    XCloseDisplay(dpy);
}
