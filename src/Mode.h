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
#ifndef XLAYOUTDISPLAY_MODE_H
#define XLAYOUTDISPLAY_MODE_H

#include <X11/extensions/Xrandr.h>

// a mode that may be used by an Xrandr display
class Mode {
public:
    Mode(const RRMode &rrMode,
         const unsigned int &width,
         const unsigned int &height,
         const unsigned int &refresh) :
            rrMode(rrMode),
            width(width),
            height(height),
            refresh(refresh) {
    }

    // order by width, height, refresh
    const bool operator<(const Mode &o) const;

    const RRMode rrMode;
    const unsigned int width;
    const unsigned int height;
    const unsigned int refresh;
};

#endif //XLAYOUTDISPLAY_MODE_H
