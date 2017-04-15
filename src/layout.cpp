#include "layout.h"
#include "laptop.h"

#include <string.h>

using namespace std;

void orderDispls(list <DisplP> &displs, const list <string> &order) {

    // stack all the preferred, available displays
    list <DisplP> preferredDispls;
    for (const auto name : order) {
        for (const auto displ : displs) {
            if (strcasecmp(name.c_str(), displ->name.c_str()) == 0) {
                preferredDispls.push_front(displ);
            }
        }
    }

    // move preferred to the front
    for (const auto preferredDispl : preferredDispls) {
        displs.remove(preferredDispl);
        displs.push_front(preferredDispl);
    }
}

void activateDispls(std::list<DisplP> &displs, const bool &lidClosed, const string &primary) {
    for (const auto displ : displs) {

        // don't use any laptop displays if the lid is closed
        if (shouldDisableDisplay(displ->name, lidClosed))
            continue;

        // only activate currently active or connected displays
        if (displ->state != Displ::active && displ->state != Displ::connected)
            continue;

        // mark active
        displ->desiredActive = true;

        // default first to primary
        if (!Displ::desiredPrimary)
            Displ::desiredPrimary = displ;

        // user selected primary
        if (!primary.empty() && strcasecmp(primary.c_str(), displ->name.c_str()) == 0)
            Displ::desiredPrimary = displ;
    }
}

void ltrDispls(list <DisplP> &displs) {

    int xpos = 0;
    int ypos = 0;
    for (const auto displ : displs) {

        if (displ->desiredActive) {

            // set the desired mode to optimal
            displ->desiredMode = displ->optimalMode;

            // position the screen
            displ->desiredPos = make_shared<Pos>(xpos, ypos);

            // next position
            xpos += displ->desiredMode->width;
        }
    }
}
