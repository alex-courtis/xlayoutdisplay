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
#ifndef XLAYOUTDISPLAY_CALCULATIONS_H
#define XLAYOUTDISPLAY_CALCULATIONS_H

#include <vector>
#include <map>
#include "Output.h"

#define DEFAULT_DPI 96

// reorder outputs putting those whose names match order at the front, case insensitive
const std::list<std::shared_ptr<Output>> orderOutputs(const std::list<std::shared_ptr<Output>> &outputs, const std::vector<std::string> &order);

// mark outputs that should be activated and return the nonempty primary
// throws invalid_argument:
//   when outputs is empty
const std::shared_ptr<Output> activateOutputs(const std::list<std::shared_ptr<Output>> &outputs,
                                              const std::string &desiredPrimary, const Monitors &monitors);

// arrange outputs left to right at optimal mode; copy all outputs that should be copied; will mutate contents
void ltrOutputs(const std::list<std::shared_ptr<Output>> &outputs, const std::map<std::string, std::string>& copies);

// arrange outputs so that they all mirror at highest common mode; will mutate contents
// throws runtime_error:
//   no common mode found
void mirrorOutputs(const std::list<std::shared_ptr<Output>> &outputs);

// render a user readable string explaining the current state of outputs
const std::string renderUserInfo(const std::list<std::shared_ptr<Output>> &outputs);

// calculate the DPI for the output given
// use only the horiz/vert cm values from EDID - they are intentionally zero for projectors and some tvs
// if horiz/vert values are unavailable or zero, return DEFAULT_DPI
// throws invalid_argument:
//   when output is empty
long calculateDpi(const std::shared_ptr<Output> &output, std::string *explaination);

// retrieve the highest resolution/refresh mode from a list of modes, using the highest refresh rate of preferredMode, if available
const std::shared_ptr<const Mode> calculateOptimalMode(const std::list<std::shared_ptr<const Mode>> &modes,
                                                       const std::shared_ptr<const Mode> &preferredMode);

#endif //XLAYOUTDISPLAY_CALCULATIONS_H
