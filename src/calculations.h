#ifndef XLAYOUTDISPLAYS_CALCULATIONS_H
#define XLAYOUTDISPLAYS_CALCULATIONS_H

#include "Displ.h"

#define DEFAULT_DPI 96

// reorder displs putting those whose names match order at the front, case insensitive
void orderDispls(std::list<std::shared_ptr<Displ>> &displs, const std::list<std::string> &order);

// mark displays that should be activated and return the nonempty primary
// throws invalid_argument:
//   when displs is empty
const std::shared_ptr<Displ> activateDispls(const std::list<std::shared_ptr<Displ>> &displs, const std::string &desiredPrimary, const Monitors &monitors);

// arrange displays left to right at optimal mode; will mutate contents
void ltrDispls(std::list<std::shared_ptr<Displ>> &displs);

// arrange displays so that they all mirror at highest common mode; will mutate contents
// throws runtime_error:
//   no common mode found
void mirrorDispls(std::list<std::shared_ptr<Displ>> &displs);

// render a user readable string explaining the current state of displs
const std::string renderUserInfo(const std::list<std::shared_ptr<Displ>> &displs);

// TODO: document and test this; refactor needed
// throws invalid_argument:
//   when displ is empty
const long calculateDpi(const std::shared_ptr<Displ> &displ, std::string &explaination);

// retrieve the highest resolution/refresh mode from a list of modes, using the highest refresh rate of preferredMode, if available
const std::shared_ptr<Mode> calculateOptimalMode(const std::list<std::shared_ptr<Mode>> &modes,
                                                 const std::shared_ptr<Mode> &preferredMode);

#endif //XLAYOUTDISPLAYS_CALCULATIONS_H
