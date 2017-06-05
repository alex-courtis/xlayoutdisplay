#ifndef XLAYOUTDISPLAYS_POS_H
#define XLAYOUTDISPLAYS_POS_H

#include <memory>

// position of a display
class Pos {
public:
    Pos(const int &x, const int &y) :
            x(x), y(y) {
    }

    const int x = 0;
    const int y = 0;
};

typedef std::shared_ptr<Pos> PosP;

#endif //XLAYOUTDISPLAYS_POS_H
