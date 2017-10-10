#ifndef XLAYOUTDISPLAYS_CALCULATIONS_H
#define XLAYOUTDISPLAYS_CALCULATIONS_H

#include "Displ.h"

// reorder displs putting those whose names match order at the front, case insensitive
void orderDispls(std::list<std::shared_ptr<Displ>> &displs, const std::list<std::string> &order);

// mark displays that should be activated and return the one and only primary
const std::shared_ptr<Displ> activateDispls(std::list<std::shared_ptr<Displ>> &displs, const std::string &desiredPrimary, const Monitors &monitors);

// arrange desiredActive displays left to right at optimal mode; will mutate contents
void ltrDispls(std::list<std::shared_ptr<Displ>> &displs);

// arrange desiredActive displays so that they all mirror at lowest common mode; will mutate contents
// throws runtime_error if no common mode found
void mirrorDispls(std::list<std::shared_ptr<Displ>> &displs);

// TODO: test
std::string calculateDpi(const std::list<std::shared_ptr<Displ>> &displs, const std::shared_ptr<Displ> &primary);

// TODO: test
// generate an optimal mode from a sorted list of modes and preferredMode
std::shared_ptr<Mode>
generateOptimalMode(const std::list<std::shared_ptr<Mode>> &modes, const std::shared_ptr<Mode> &preferredMode);

#endif //XLAYOUTDISPLAYS_CALCULATIONS_H
