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
#ifndef XLAYOUTDISPLAY_XRANDRUTIL_H
#define XLAYOUTDISPLAY_XRANDRUTIL_H

#include "Output.h"

// v refresh frequency in even Hz, zero if modeInfo is NULL
unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo);

// render xrandr cmd to layout outputs
// will activate only if desiredActive, desiredMode, desiredPos are set
// desiredPrimary is only set if activated
const std::string renderXrandrCmd(const std::list<std::shared_ptr<Output>> &outputs, const std::shared_ptr<Output> &primary, const long &dpi, const long &rate);

// throws invalid_argument:
//   null resources
//   id not found in resources
Mode *modeFromXRR(RRMode id, const XRRScreenResources *resources);

// build a list of Output based on the current and possible state of the world
const std::list<std::shared_ptr<Output>> discoverOutputs();

#endif //XLAYOUTDISPLAY_XRANDRUTIL_H
