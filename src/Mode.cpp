#include "Mode.h"

#include "xrandrrutil.h"

using namespace std;

Mode::Mode(const RRMode id, const XRRScreenResources *resources) {

    if (resources == NULL)
        throw invalid_argument("cannot construct Mode: NULL XRRScreenResources");

    XRRModeInfo *modeInfo = NULL;
    for (int i = 0; i < resources->nmode; i++) {
        if (id == resources->modes[i].id) {
            modeInfo = &(resources->modes[i]);
            break;
        }
    }

    if (modeInfo == NULL)
        throw invalid_argument("cannot construct Mode: cannot retrieve RRMode '" + to_string(id) + "'");

    rrMode = id;
    width = modeInfo->width;
    height = modeInfo->height;
    refresh = refreshFromModeInfo(modeInfo);
}

bool Mode::operator<(const Mode &o) {
    if (width == o.width)
        if (refresh == o.refresh)
            return height > o.height;
        else
            return refresh > o.refresh;
    else
        return width > o.width;
}
