#include <strings.h>
#include "layout.h"

using namespace std;

// reorder displs putting those whose names match order at the front
void orderDispls(list <DisplP> &displs, const list <string> &order) {

    // stack all the preferred, available displays
    list <DisplP> preferredDispls;
    for (const auto name : order) {
        for (const auto displ : displs) {
            if (name == displ->name) {
                preferredDispls.push_front(displ);
            }
        }
    }

    // move preferred to the front
    for (const auto preferredDispl : preferredDispls) {
        for (const auto displ : displs) {
            if (strcasecmp(displ->name, preferredDispl->name) == 0) {
                displs.remove(displ);
                displs.push_front(displ);
                break;
            }
        }
    }
}
