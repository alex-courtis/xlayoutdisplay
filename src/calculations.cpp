/*
   Copyright 2018 Alexander Courtis

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "calculations.h"
#include "util.h"

#include <sstream>
#include <cstring>
#include <stack>
#include <system_error>
#include <algorithm>

using namespace std;

const std::list<std::shared_ptr<Output>> orderOutputs(const list<shared_ptr<Output>> &outputs, const vector<string> &order) {
    list<shared_ptr<Output>> orderedOutputs(outputs);

    // stack all the preferred, available outputs
    stack<shared_ptr<Output>> preferredOutputs;
    for (const auto &name : order) {
        for (const auto &output : outputs) {
            if (strcasecmp(name.c_str(), output->name.c_str()) == 0) {
                preferredOutputs.push(output);
            }
        }
    }

    // move preferred to the front
    while (!preferredOutputs.empty()) {
        orderedOutputs.remove(preferredOutputs.top());
        orderedOutputs.push_front(preferredOutputs.top());
        preferredOutputs.pop();
    }

    return orderedOutputs;
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
        output->desiredActive = true;

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

void ltrOutputs(const list<shared_ptr<Output>> &outputs, const map<string, string> &copies) {
    int xpos = 0;
    int ypos = 0;
    for (const auto &output : outputs) {

        if (output->desiredActive && copies.find(output->name) == copies.end()) {

            // set the desired mode to optimal
            output->desiredMode = output->optimalMode;

            // position the screen
            output->desiredPos = make_shared<Pos>(xpos, ypos);

            // next position
            xpos += output->desiredMode->width;
        }
    }
    for (const auto &entry : copies) {
        // find target to copy and output to put on
        auto output = *find_if(outputs.begin(), outputs.end(), 
            [&](auto &output){ return output->name == entry.first;}
        );
        auto target = *find_if(outputs.begin(), outputs.end(),
            [&](auto &output){ return output->name == entry.second;}
        );

        if (output->desiredActive && target->desiredActive) {

            // find supported mode of output that has same size of target
            // with highest refresh rate
            shared_ptr<const Mode> currentMode = nullptr;
            for (const auto &mode : output->modes){
                if (mode->width == target->desiredMode->width && mode->height == target->desiredMode->height) {
                    if (currentMode == nullptr || currentMode->refresh < mode->refresh) {
                        currentMode = mode;
                    }
                }
            }

            if (currentMode == nullptr) {
                throw runtime_error("no matching mode find for copying " + target->name + " on " + output->name);
            }

            // copy modes
            output->desiredMode = currentMode;

            // copy positions
            output->desiredPos = target->desiredPos;
        }
    }
}

void mirrorOutputs(const list<shared_ptr<Output>> &outputs) {

    // find the first active output
    shared_ptr<Output> firstOutput;
    for (const auto &output : outputs) {
        if (output->desiredActive) {
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
            if (!output->desiredActive)
                continue;

            // reset failed matches
            shared_ptr<const Mode> desiredMode;

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
                output->desiredMode = desiredMode;
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

long calculateDpi(const shared_ptr<Output> &output, string *explaination) {
    if (!output) throw invalid_argument("calculateDpi received empty output");

    long dpi = DEFAULT_DPI;
    stringstream verbose;
    if (!output->edid) {
        verbose << "DPI defaulting to "
                << dpi
                << "; EDID information not available for output "
                << output->name;
    } else if (!output->desiredMode) {
        verbose << "DPI defaulting to "
                << dpi
                << "; no desired mode for output "
                << output->name;
    } else {
        const long caldulatedDpi = output->edid->dpiForMode(output->desiredMode);
        if (caldulatedDpi == 0) {
            verbose << "DPI defaulting to "
                    << dpi
                    << " as no EDID horiz/vert cm information available for "
                    << output->name
                    << "; this is normal for projectors and some TVs";
        } else {
            dpi = caldulatedDpi;
            verbose << "calculated DPI "
                    << dpi
                    << " for output "
                    << output->name;
        }
    }

    *explaination = verbose.str();
    return dpi;
}

const shared_ptr<const Mode> calculateOptimalMode(const list<shared_ptr<const Mode>> &modes, const shared_ptr<const Mode> &preferredMode) {
    shared_ptr<const Mode> optimalMode;

    // default optimal mode is empty
    if (!modes.empty()) {

        // use highest resolution/refresh for optimal
        const list<shared_ptr<const Mode>> reverseOrderedModes = reverseSort(modes);
        optimalMode = reverseOrderedModes.front();

        // override with highest refresh of preferred resolution, if available
        if (preferredMode)
            for (const auto &mode : reverseOrderedModes)
                if (mode->width == preferredMode->width && mode->height == preferredMode->height) {
                    optimalMode = mode;
                    break;
                }
    }

    return optimalMode;
}
