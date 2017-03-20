cmake_minimum_required(VERSION 3.6)
project(xLayoutDisplays)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
link_libraries(${X11_Xrandr_LIB})

set(SOURCE_FILES xLayoutDisplays.cpp)

add_executable(xLayoutDisplays ${SOURCE_FILES})
