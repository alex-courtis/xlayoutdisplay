#ifndef XLAYOUTDISPLAYS_XRANDRUTIL_H
#define XLAYOUTDISPLAYS_XRANDRUTIL_H

#include "Displ.h"

// v refresh frequency in even Hz, zero if modeInfo is NULL
const unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo);

// render xrandr cmd to layout displays
// will activate only if desiredActive, desiredMode, desiredPos are set
// desiredPrimary is only set if activated
// DPI will be set to that of the desiredPrimary which specifies it, otherwise 96
const std::string renderXrandrCmd(const std::list<std::shared_ptr<Displ>> &displs, const std::shared_ptr<Displ> &primary, const long &dpi);

// render a user readable string explaining the current state of displs
const std::string renderUserInfo(const std::list<std::shared_ptr<Displ>> &displs);

// throws invalid_argument:
//   null resources
//   id not found in resources
Mode *modeFromXRR(RRMode id, const XRRScreenResources *resources);

// build a list of Displ based on the current and possible state of the world
// throws runtime_error:
//   failed to open display defined by DISPLAY environment variable
//   default X screen is outside the X screen count
const std::list<std::shared_ptr<Displ>> discoverDispls();

#endif //XLAYOUTDISPLAYS_XRANDRUTIL_H
