#include "xrandrrutil.h"

const XRRModeInfo *modeInfoFromId(const RRMode id, const XRRScreenResources *resources) {
    if (resources)
        for (int i = 0; i < resources->nmode; i++)
            if (id == resources->modes[i].id)
                return &(resources->modes[i]);
    return NULL;
}

// stolen from xrandr.c; assuming this mostly works
const unsigned int refreshFromModeInfo(const XRRModeInfo *modeInfo) {
    if (!modeInfo) return 0;

    double rate;
    double vTotal = modeInfo->vTotal;

    /* doublescan doubles the number of lines */
    if (modeInfo->modeFlags & RR_DoubleScan)
        vTotal *= 2;

    /* interlace splits the frame into two fields */
    /* the field rate is what is typically reported by monitors */
    if (modeInfo->modeFlags & RR_Interlace)
        vTotal /= 2;

    if (modeInfo->hTotal && vTotal)
        rate = ((double) modeInfo->dotClock / (modeInfo->hTotal * vTotal));
    else
        rate = 0;

    // round up, as xrandr uses the greatest rate less than passed
    return (unsigned int) (rate + 0.5);
}
