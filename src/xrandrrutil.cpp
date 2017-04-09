#include "xrandrrutil.h"

#include <sstream>

using namespace std;

// stolen from xrandr.c; assuming this mostly works
const unsigned int refreshFromModeInfo(const XRRModeInfo &modeInfo) {
    double rate;
    double vTotal = modeInfo.vTotal;

    /* doublescan doubles the number of lines */
    if (modeInfo.modeFlags & RR_DoubleScan)
        vTotal *= 2;

    /* interlace splits the frame into two fields */
    /* the field rate is what is typically reported by monitors */
    if (modeInfo.modeFlags & RR_Interlace)
        vTotal /= 2;

    if (modeInfo.hTotal && vTotal)
        rate = ((double) modeInfo.dotClock / (modeInfo.hTotal * vTotal));
    else
        rate = 0;

    // round up, as xrandr uses the greatest rate less than passed
    return (unsigned int) (rate + 0.5);
}

const string renderCmd(const list <DisplP> &displs) {
    stringstream ss;
    ss << "xrandr";
    for (const auto displ : displs) {
        ss << " \\\n";
        ss << " --output " << displ->name;
        if (displ->desiredActive && displ->desiredMode && displ->desiredPos) {
            ss << " --mode " << displ->desiredMode->width << "x" << displ->desiredMode->height;
            ss << " --rate " << displ->desiredMode->refresh;
            ss << " --pos ";
            ss << displ->desiredPos->x << "x" << displ->desiredPos->y;
            if (displ == Displ::desiredPrimary) {
                ss << " --primary";
            }
        } else {
            ss << " --off";
        }
    }
    return ss.str();
}
