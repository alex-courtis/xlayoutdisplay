/*
   Copyright 2018 Alexander Courtis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include <X11/Xcursor/Xcursor.h>
#include <proc/readproc.h>
#include <cstring>

bool xorgRunning() {
	proc_t **pptr = readproctab(PROC_FILLSTAT);

    for (; *pptr; pptr++) {
        proc_t *p = *pptr;
        if (strcmp(p->cmd, "Xorg") == 0) {
            return true;
        }
    }

    return false;
}

void resetRootCursor() {
    Display *dpy = XOpenDisplay(nullptr);
    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);
    Cursor cursor = XcursorLibraryLoadCursor(dpy, "left_ptr");
    XDefineCursor(dpy, root, cursor);
    XFreeCursor(dpy, cursor);
    XCloseDisplay(dpy);
}
