#ifndef XLAYOUTDISPLAYS_XRANDRUTIL_H
#define XLAYOUTDISPLAYS_XRANDRUTIL_H

#include "XrrWrapper.h"
#include "Displ.h"

// v refresh frequency in even Hz, zero if modeInfo is NULL
const unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo);

// render xrandr cmd to layout displays
// will activate only if desiredActive, desiredMode, desiredPos are set
// desiredPrimary is only set if activated
// DPI will be set to that of the desiredPrimary which specifies it, otherwise 96
const std::string renderCmd(const std::list<DisplP> &displs);

// render a user readable string explaining the current state of displs
const std::string renderUserInfo(const std::list<DisplP> &displs);

// build a list of Displ based on the current and possible state of the world; subclassed XrrWrapper used for testing
const std::list<DisplP> discoverDispls(XrrWrapper *xrrWrapper = NULL);

#endif //XLAYOUTDISPLAYS_XRANDRUTIL_H
