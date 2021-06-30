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
#ifndef XLAYOUTDISPLAY_STDUTIL_H
#define XLAYOUTDISPLAY_STDUTIL_H

#include <memory>
#include <climits>

// sorting function for shared pointers... this must be in STL somewhere...
template<typename T>
inline bool sortSharedPtr(const std::shared_ptr<T> &l, const std::shared_ptr<T> &r) {
    return (*l) < (*r);
}

// copy list of shared_ptr, reverse sort it, return it as const
template<typename T>
inline const std::list<std::shared_ptr<const T>> reverseSort(const std::list<std::shared_ptr<const T>> &list) {
    std::list<std::shared_ptr<const T>> sorted = list;
    sorted.sort(sortSharedPtr<const T>);
    sorted.reverse();
    return sorted;
}

// return an absolute UNIX path for a relative path under env
inline const std::string resolveEnvPath(const char *env, const char *homeRelativePath) {
    char settingsFilePath[PATH_MAX];
    snprintf(settingsFilePath, PATH_MAX, "%s/%s", getenv(env), homeRelativePath);
    return std::string(settingsFilePath);
}

#endif //XLAYOUTDISPLAY_STDUTIL_H
