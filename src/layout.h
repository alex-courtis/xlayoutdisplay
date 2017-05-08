#ifndef XLAYOUTDISPLAYS_LAYOUT_H
#define XLAYOUTDISPLAYS_LAYOUT_H

#include "Displ.h"

// reorder displs putting those whose names match order at the front, case insensitive
void orderDispls(std::list<DisplP> &displs, const std::list<std::string> &order);

// mark displays that should be activated and set the one and only primary
void activateDispls(std::list<DisplP> &displs, const std::string &primary);

// arrange desiredActive displays left to right at optimal mode; will mutate contents
void ltrDispls(std::list<DisplP> &displs);

// arrange desiredActive displays so that they all mirror at lowest common mode; will mutate contents
// throws runtime_error if no common mode found
void mirrorDispls(std::list<DisplP> &displs);

std::string calculateDpi(std::list<DisplP> &displs);

#endif //XLAYOUTDISPLAYS_LAYOUT_H
