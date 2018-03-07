#ifndef XLAYOUTDISPLAY_POS_H
#define XLAYOUTDISPLAY_POS_H

#include <memory>

// position of an output
class Pos {
public:
    Pos(const int &x, const int &y) :
            x(x), y(y) {
    }

    const int x = 0;
    const int y = 0;
};

#endif //XLAYOUTDISPLAY_POS_H
