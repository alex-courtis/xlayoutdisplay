#ifndef XLAYOUTDISPLAYS_LAYOUT_H
#define XLAYOUTDISPLAYS_LAYOUT_H

#include "Displ.h"

// reorder displs putting those whose names match order at the front
void orderDispls(std::list <DisplP> &displs, const std::list <std::string> &order);

#endif //XLAYOUTDISPLAYS_LAYOUT_H
