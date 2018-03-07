#include "calculations.h"
#include "util.h"

#include <sstream>
#include <cstring>

using namespace std;

void orderOutputs(list<shared_ptr<Output>> &outputs, const list<string> &order) {

    // stack all the preferred, available outputs
    list<shared_ptr<Output>> preferredOutputs;
    for (const auto &name : order) {
        for (const auto &output : outputs) {
            if (strcasecmp(name.c_str(), output->name.c_str()) == 0) {
                preferredOutputs.push_front(output);
            }
        }
    }

    // move preferred to the front
    for (const auto &preferredOutput : preferredOutputs) {
        outputs.remove(preferredOutput);
        outputs.push_front(preferredOutput);
    }
}

const shared_ptr<Output> activateOutputs(const list<shared_ptr<Output>> &outputs, const string &desiredPrimary,
                                         const Monitors &monitors) {
    if (outputs.empty()) throw invalid_argument("activateOutputs received empty outputs");

    shared_ptr<Output> primary;

    for (const auto &output : outputs) {

        // don't activate any monitors that shouldn't
        if (monitors.shouldDisableOutput(output->name))
            continue;

        // only activate currently active or connected ouputs
        if (output->state != Output::active && output->state != Output::connected)
            continue;

        // mark active
        output->desiredActive(true);

        // default first to primary
        if (!primary)
            primary = output;

        // user selected primary
        if (!desiredPrimary.empty() && strcasecmp(desiredPrimary.c_str(), output->name.c_str()) == 0)
            primary = output;
    }

    if (!primary) throw runtime_error("no active or connected outputs found");

    return primary;
}

void ltrOutputs(list<shared_ptr<Output>> &outputs) {
    int xpos = 0;
    int ypos = 0;
    for (const auto &output : outputs) {

        if (output->desiredActive()) {

            // set the desired mode to optimal
            output->desiredMode(output->optimalMode);

            // position the screen
            output->desiredPos = make_shared<Pos>(xpos, ypos);

            // next position
            xpos += output->desiredMode()->width;
        }
    }
}

void mirrorOutputs(list<shared_ptr<Output>> &outputs) {

    // find the first active output
    shared_ptr<Output> firstOutput;
    for (const auto &output : outputs) {
        if (output->desiredActive()) {
            firstOutput = output;
            break;
        }
    }
    if (!firstOutput)
        return;

    // iterate through first active output's modes
    for (const auto &possibleMode : reverseSort(firstOutput->modes)) {
        bool matched = true;

        // attempt to match mode to each active output
        for (const auto &output : outputs) {
            if (!output->desiredActive())
                continue;

            // reset failed matches
            shared_ptr<Mode> desiredMode;

            // match height and width
            for (const auto &mode : reverseSort(output->modes)) {
                if (mode->width == possibleMode->width && mode->height == possibleMode->height) {

                    // select best refresh
                    desiredMode = mode;
                    break;
                }
            }

            // match a mode for every output; root it at 0, 0
            matched = matched && desiredMode;
            if (matched) {
                output->desiredMode(desiredMode);
                output->desiredPos = make_shared<Pos>(0, 0);
                continue;
            }
        }

        // we've set desiredMode and desiredPos (zero) for all outputs, all done
        if (matched)
            return;
    }

    // couldn't find a common mode, exit
    throw runtime_error("unable to find common width/height for mirror");
}

const string renderUserInfo(const list<shared_ptr<Output>> &outputs) {
    stringstream ss;
    for (const auto &output : outputs) {
        ss << output->name;
        switch (output->state) {
            case Output::active:
                ss << " active";
                break;
            case Output::connected:
                ss << " connected";
                break;
            case Output::disconnected:
                ss << " disconnected";
                break;
        }
        if (output->edid) {
            ss << ' ' << output->edid->maxCmHoriz() << "cm/" << output->edid->maxCmVert() << "cm";
        }
        if (output->currentMode && output->currentPos) {
            ss << ' ' << output->currentMode->width << 'x' << output->currentMode->height;
            ss << '+' << output->currentPos->x << '+' << output->currentPos->y;
            ss << ' ' << output->currentMode->refresh << "Hz";
        }
        ss << endl;
        for (const auto &mode : output->modes) {
            ss << (mode == output->currentMode ? '*' : ' ');
            ss << (mode == output->preferredMode ? '+' : ' ');
            ss << (mode == output->optimalMode ? '!' : ' ');
            ss << mode->width << 'x' << mode->height << ' ' << mode->refresh << "Hz";
            ss << endl;
        }
    }
    ss << "*current +preferred !optimal";
    return ss.str();
}

const long calculateDpi(const shared_ptr<Output> &output, string &explaination) {
    if (!output) throw invalid_argument("calculateDpi received empty output");

    long dpi = DEFAULT_DPI;
    stringstream verbose;
    if (!output->edid) {
        verbose << "DPI defaulting to "
                << dpi
                << "; EDID information not available for output "
                << output->name;
    } else if (!output->desiredMode()) {
        // TODO is this a realistic case? no, we need an "ActiveOutput" subclass of Output
        verbose << "DPI defaulting to "
                << dpi
                << "; desiredMode not available for output "
                << output->name;
    } else {
        const long caldulatedDpi = output->edid->dpiForMode(output->desiredMode());
        if (caldulatedDpi == 0) {
            verbose << "DPI defaulting to "
                    << dpi
                    << "; no EDID size information available for "
                    << output->name;
        } else {
            dpi = caldulatedDpi;
            verbose << "calculated DPI "
                    << dpi
                    << " for output "
                    << output->name;
        }
    }

    explaination = verbose.str();
    return dpi;
}

const shared_ptr<Mode> calculateOptimalMode(const list<shared_ptr<Mode>> &modes, const shared_ptr<Mode> &preferredMode) {
    shared_ptr<Mode> optimalMode;

    // default optimal mode is empty
    if (!modes.empty()) {

        // use highest resolution/refresh for optimal
        const list<shared_ptr<Mode>> reverseOrderedModes = reverseSort(modes);
        optimalMode = reverseOrderedModes.front();

        // override with highest refresh of preferred resolution, if available
        if (preferredMode)
            for (auto &mode : reverseOrderedModes)
                if (mode->width == preferredMode->width && mode->height == preferredMode->height) {
                    optimalMode = mode;
                    break;
                }
    }

    return optimalMode;
}
