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
#ifndef XLAYOUTDISPLAY_XUTIL_H
#define XLAYOUTDISPLAY_XUTIL_H

// reset the cursor to "left_ptr" cursor on the root window
// takes into account new Xft.dpi as well as user Xcursor theme/size settings
void resetRootCursor();

#endif //XLAYOUTDISPLAY_XUTIL_H
