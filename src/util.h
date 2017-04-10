#ifndef XLAYOUTDISPLAYS_STDUTIL_H
#define XLAYOUTDISPLAYS_STDUTIL_H

#include <memory>

// sorting function for shared pointers... this must be in STL somewhere...
template<typename T>
bool sortSharedPtr(const std::shared_ptr<T> &l, const std::shared_ptr<T> &r) {
    return (*l) < (*r);
}

#endif //XLAYOUTDISPLAYS_STDUTIL_H
