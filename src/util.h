#ifndef XLAYOUTDISPLAYS_STDUTIL_H
#define XLAYOUTDISPLAYS_STDUTIL_H

#include <memory>

// sorting function for shared pointers... this must be in STL somewhere...
template<typename T>
inline bool sortSharedPtr(const std::shared_ptr<T> &l, const std::shared_ptr<T> &r) {
    return (*l) < (*r);
}

// copy shared_ptr list, sort it, reverse it, return it
template<typename T>
inline const std::list<std::shared_ptr<T>> reverseSortListSharedPtr(const std::list<std::shared_ptr<T>> &list) {
    std::list<std::shared_ptr<T>> sortedReversed = list;
    sortedReversed.sort(sortSharedPtr<T>);
    sortedReversed.reverse();
    return sortedReversed;
}

#endif //XLAYOUTDISPLAYS_STDUTIL_H
