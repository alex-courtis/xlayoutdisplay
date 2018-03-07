#ifndef XLAYOUTDISPLAY_XRANDRUTIL_H
#define XLAYOUTDISPLAY_XRANDRUTIL_H

#include "Output.h"

// v refresh frequency in even Hz, zero if modeInfo is NULL
const unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo);

// render xrandr cmd to layout outputs
// will activate only if desiredActive, desiredMode, desiredPos are set
// desiredPrimary is only set if activated
const std::string renderXrandrCmd(const std::list<std::shared_ptr<Output>> &outputs, const std::shared_ptr<Output> &primary, const long &dpi);

// throws invalid_argument:
//   null resources
//   id not found in resources
Mode *modeFromXRR(RRMode id, const XRRScreenResources *resources);

// build a list of Output based on the current and possible state of the world
const std::list<std::shared_ptr<Output>> discoverOutputs();

#endif //XLAYOUTDISPLAY_XRANDRUTIL_H
