#ifndef XLAYOUTDISPLAYS_LAYOUT_H
#define XLAYOUTDISPLAYS_LAYOUT_H

#include "Displ.h"
#include "Laptop.h"
#include "Settings.h"

// contains all information necessary for the displays to be laid out
class Layout {
public:
    // todo: construct displs, laptop etc in here
    Layout(const std::list<DisplP> &displs);

    virtual ~Layout() {
    }

    // reorder displs putting those whose names match order at the front, case insensitive
    void orderDispls(const std::list<std::string> &order);

    // mark displays that should be activated and set the one and only primary
    void activateDispls(const std::string &primary);

    // arrange desiredActive displays left to right at optimal mode; will mutate contents
    void ltrDispls();

    // arrange desiredActive displays so that they all mirror at lowest common mode; will mutate contents
    // throws runtime_error if no common mode found
    void mirrorDispls();

    // todo: document and test this
    std::string calculateDpi();

//private:
    // todo: make private
    std::list<DisplP> displs;
};

#endif //XLAYOUTDISPLAYS_LAYOUT_H
