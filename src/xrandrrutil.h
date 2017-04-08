#ifndef XLAYOUTDISPLAYS_XRANDRUTIL_H
#define XLAYOUTDISPLAYS_XRANDRUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <X11/extensions/Xrandr.h>

// v refresh frequency in even Hz, zero if modeInfo is NULL
const unsigned int refreshFromModeInfo(const XRRModeInfo *modeInfo);

// find the mode info for the id passed, in resources, NULL if not found
const XRRModeInfo *modeInfoFromId(const RRMode id, const XRRScreenResources *resources);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //XLAYOUTDISPLAYS_XRANDRUTIL_H
