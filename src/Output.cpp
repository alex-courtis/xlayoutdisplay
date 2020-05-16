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
#include "Output.h"
#include "calculations.h"

#include <algorithm>
#include <system_error>

using namespace std;

Output::Output(const string &name,
             const State &state,
             const list<shared_ptr<const Mode>> &modes,
             const shared_ptr<const Mode> &currentMode,
             const shared_ptr<const Mode> &preferredMode,
             const shared_ptr<const Pos> &currentPos,
             const shared_ptr<const Edid> &edid) :
        name(name),
        state(state),
        modes(modes),
        currentMode(currentMode),
        preferredMode(preferredMode),
        optimalMode(calculateOptimalMode(modes, preferredMode)),
        currentPos(currentPos),
        edid(edid) {
    switch (state) {
        case active:
            if (!currentMode) throw invalid_argument("active Output '" + name + "' has no currentMode");
            if (!currentPos) throw invalid_argument("active Output '" + name + "' has no currentPos");
            if (modes.empty()) throw invalid_argument("active Output '" + name + "' has no modes");
            break;
        case connected:
            if (modes.empty()) throw invalid_argument("connected Output '" + name + "' has no modes");
            break;
        default:
            break;
    }

    // active / connected must have NULL or valid preferred mode
    if (state == active || state == connected) {
        if (preferredMode && find(this->modes.begin(), this->modes.end(), preferredMode) == this->modes.end())
            throw invalid_argument("Output '" + name + "' has preferredMode not present in modes");
    }
}

