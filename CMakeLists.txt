cmake_minimum_required(VERSION 3.8)

project(xLayoutDisplays)

set(CMAKE_CXX_STANDARD 17)

find_package(X11 REQUIRED)

link_libraries(${X11_LIBRARIES})
link_libraries(${X11_Xrandr_LIB})

file(GLOB SRC_FILES "src/*")

add_executable(xLayoutDisplays main.cpp ${SRC_FILES})

file(GLOB TEST_FILES "test/*")

add_executable(Tests ${SRC_FILES} ${TEST_FILES})

target_link_libraries(Tests gtest gmock gmock_main pthread)
