#ifndef XLAYOUTDISPLAYS_ARRANGEMENT_H
#define XLAYOUTDISPLAYS_ARRANGEMENT_H

#include "Displ.h"

// reorder displs putting those whose names match order at the front, case insensitive
void orderDispls(std::list<std::shared_ptr<Displ>> &displs, const std::list<std::string> &order);

// mark displays that should be activated and set the one and only primary
void activateDispls(std::list<std::shared_ptr<Displ>> &displs, const std::string &primary, const Monitors &monitors);

// arrange desiredActive displays left to right at optimal mode; will mutate contents
void ltrDispls(std::list<std::shared_ptr<Displ>> &displs);

// arrange desiredActive displays so that they all mirror at lowest common mode; will mutate contents
// throws runtime_error if no common mode found
void mirrorDispls(std::list<std::shared_ptr<Displ>> &displs);

std::string calculateDpi(std::list<std::shared_ptr<Displ>> &displs);

#endif //XLAYOUTDISPLAYS_ARRANGEMENT_H
