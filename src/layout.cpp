#include "layout.h"
#include "Laptop.h"

#include <string.h>
#include <iostream>

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

void activateDispls(std::list<DisplP> &displs, const string &primary) {
    Laptop *laptop = Laptop::instance();
    for (const auto displ : displs) {

        // don't use any laptop displays if the lid is closed
        if (laptop->shouldDisableDisplay(displ->name))
            continue;

        // only activate currently active or connected displays
        if (displ->state != Displ::active && displ->state != Displ::connected)
            continue;

        // mark active
        displ->setDesiredActive();

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

        if (displ->isDesiredActive()) {

            // set the desired mode to optimal
            displ->desiredMode(displ->optimalMode);

            // position the screen
            displ->desiredPos = make_shared<Pos>(xpos, ypos);

            // next position
            xpos += displ->desiredMode()->width;
        }
    }
}

void mirrorDispls(list <DisplP> &displs) {

    // find the first active display
    DisplP firstDispl;
    for (const auto displ : displs) {
        if (displ->isDesiredActive()) {
            firstDispl = displ;
            break;
        }
    }
    if (!firstDispl)
        return;

    // iterate through first active display's modes
    for (const auto possibleMode : firstDispl->modes) {
        bool matched = true;

        // attempt to match mode to each active displ
        for (const auto displ : displs) {
            if (!displ->isDesiredActive())
                continue;

            // reset failed matches
            ModeP desiredMode;

            // match height and width only
            for (const auto mode : displ->modes) {
                if (mode->width == possibleMode->width && mode->height == possibleMode->height) {

                    // set mode and pos
                    desiredMode = mode;
                    break;
                }
           }

            // match a mode for every display; root it at 0, 0
            matched = matched && desiredMode;
            if (matched) {
                displ->desiredMode(desiredMode);
                displ->desiredPos = make_shared<Pos>(0, 0);
                continue;
            }
        }

        // we've set desiredMode and desiredPos (zero) for all displays, all done
        if (matched)
            return;
    }

    // couldn't find a common mode, exit
    throw runtime_error("unable to find common width/height for mirror");
}
