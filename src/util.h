#ifndef XLAYOUTDISPLAYS_STDUTIL_H
#define XLAYOUTDISPLAYS_STDUTIL_H

#include <memory>
#include <climits>

// sorting function for shared pointers... this must be in STL somewhere...
template<typename T>
inline bool sortSharedPtr(const std::shared_ptr<T> &l, const std::shared_ptr<T> &r) {
    return (*l) < (*r);
}

// copy list of shared_ptr, reverse sort it, return it as const
template<typename T>
inline const std::list<std::shared_ptr<T>> reverseSort(const std::list<std::shared_ptr<T>> &list) {
    std::list<std::shared_ptr<T>> sorted = list;
    sorted.sort(sortSharedPtr<T>);
    sorted.reverse();
    return sorted;
}

// return an absolute UNIX path for a relative path in the user env $HOME
inline const std::string resolveTildePath(const char *homeRelativePath) {
    char settingsFilePath[PATH_MAX];
    snprintf(settingsFilePath, PATH_MAX, "%s/%s", getenv("HOME"), homeRelativePath);
    return std::string(settingsFilePath);
}

#endif //XLAYOUTDISPLAYS_STDUTIL_H
