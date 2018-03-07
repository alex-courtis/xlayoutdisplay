#ifndef XLAYOUTDISPLAY_CALCULATIONS_H
#define XLAYOUTDISPLAY_CALCULATIONS_H

#include "Output.h"

#define DEFAULT_DPI 96

// reorder outputs putting those whose names match order at the front, case insensitive
void orderOutputs(std::list<std::shared_ptr<Output>> &outputs, const std::list<std::string> &order);

// mark outputs that should be activated and return the nonempty primary
// throws invalid_argument:
//   when outputs is empty
const std::shared_ptr<Output> activateOutputs(const std::list<std::shared_ptr<Output>> &outputs,
                                              const std::string &desiredPrimary, const Monitors &monitors);

// arrange outputs left to right at optimal mode; will mutate contents
void ltrOutputs(std::list<std::shared_ptr<Output>> &outputs);

// arrange outputs so that they all mirror at highest common mode; will mutate contents
// throws runtime_error:
//   no common mode found
void mirrorOutputs(std::list<std::shared_ptr<Output>> &outputs);

// render a user readable string explaining the current state of outputs
const std::string renderUserInfo(const std::list<std::shared_ptr<Output>> &outputs);

// TODO: document and test this; refactor needed, assume we have desiredActive following refactor
// calculate the DPI for the output given
// use only the horiz/vert cm values from EDID - they are intentionally zero for projectors and some tvs
// if horiz/vert values are unavailable or zero, return DEFAULT_DPI
// throws invalid_argument:
//   when outputs is empty
const long calculateDpi(const std::shared_ptr<Output> &output, std::string &explaination);

// retrieve the highest resolution/refresh mode from a list of modes, using the highest refresh rate of preferredMode, if available
const std::shared_ptr<Mode> calculateOptimalMode(const std::list<std::shared_ptr<Mode>> &modes,
                                                 const std::shared_ptr<Mode> &preferredMode);

#endif //XLAYOUTDISPLAY_CALCULATIONS_H
