#ifndef XLAYOUTDISPLAYS_STDUTIL_H
#define XLAYOUTDISPLAYS_STDUTIL_H

#include <memory>

// sorting function for shared pointers... this must be in STL somewhere...
template<typename T>
inline bool sortSharedPtr(const std::shared_ptr<T> &l, const std::shared_ptr<T> &r) {
    return (*l) < (*r);
}

// copy list of shared_ptr, sort it, return it as const
template<typename T>
inline const std::list<std::shared_ptr<T>> sortSharedPtrList(const std::list<std::shared_ptr<T>> &list) {
    std::list<std::shared_ptr<T>> sorted = list;
    sorted.sort(sortSharedPtr<T>);
    return sorted;
}

// copy list of shared_ptr, reverse it, return it as const
template<typename T>
inline const std::list<std::shared_ptr<T>> reverseSharedPtrList(const std::list<std::shared_ptr<T>> &list) {
    std::list<std::shared_ptr<T>> reversed = list;
    reversed.reverse();
    return reversed;
}

#endif //XLAYOUTDISPLAYS_STDUTIL_H
