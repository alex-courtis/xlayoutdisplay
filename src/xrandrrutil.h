#ifndef XLAYOUTDISPLAYS_XRANDRUTIL_H
#define XLAYOUTDISPLAYS_XRANDRUTIL_H

#include <X11/extensions/Xrandr.h>
#include "Displ.h"

// v refresh frequency in even Hz, zero if modeInfo is NULL
const unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo);

// render xrandr cmd to layout displays
// will activate only if desiredActive, desiredMode, desiredPos are set
// desiredPrimary is only set if activated
const std::string renderCmd(const std::list <DisplP> &displs);

#endif //XLAYOUTDISPLAYS_XRANDRUTIL_H
