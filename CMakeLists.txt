cmake_minimum_required(VERSION 3.8)

project(xLayoutDisplays)

set(CMAKE_CXX_STANDARD 17)

find_package(X11 REQUIRED)

link_libraries(${X11_LIBRARIES})
link_libraries(${X11_Xrandr_LIB})

set(SRC_FILES
        src/Edid.cpp
        src/Edid.h
        src/Displ.cpp
        src/Displ.h
        src/Monitors.cpp
        src/Monitors.h
        src/Layout.cpp
        src/Layout.h
        src/Mode.cpp
        src/Mode.h
        src/Pos.h
        src/Settings.cpp
        src/Settings.h
        src/util.h
        src/xrandrrutil.cpp
        src/xrandrrutil.h
        src/xrdbutil.h
        src/xrdbutil.cpp
        )

add_executable(xLayoutDisplays src/main.cpp ${SRC_FILES})

set(TEST_FILES
        test/test-Edid.cpp
        test/test-Displ.cpp
        test/test-Monitors.cpp
        test/test-Mode.cpp
        test/test-Settings.cpp
        test/test-xrandrutil.cpp
        test/test-xrdbutil.cpp
        )

add_executable(Tests test/main.cpp ${SRC_FILES} ${TEST_FILES})

target_link_libraries(Tests gtest)
target_link_libraries(Tests gmock)
target_link_libraries(Tests pthread)
