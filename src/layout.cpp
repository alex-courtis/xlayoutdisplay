#include <strings.h>
#include "layout.h"

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
