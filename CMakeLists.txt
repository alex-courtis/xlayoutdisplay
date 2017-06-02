cmake_minimum_required(VERSION 3.6)
project(xLayoutDisplays)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
link_libraries(${X11_Xrandr_LIB})

# need to discover how to link just to exe... sounds like multiple cmake files would be the way
link_libraries(gtest)
link_libraries(gmock)
link_libraries(pthread) # used for gmock

set(SOURCE_FILES
        src/Edid.cpp
        src/Edid.h
        src/Displ.cpp
        src/Displ.h
        src/Laptop.cpp
        src/Laptop.h
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
        )

set(TEST_FILES
        test/main.cpp
        test/test-Edid.cpp
        test/test-Displ.cpp
        test/test-Laptop.cpp
        test/test-Mode.cpp
        test/test-Settings.cpp
        test/test-xrandrutil.cpp
        )

add_executable(gtests ${SOURCE_FILES} ${TEST_FILES})

add_executable(xLayoutDisplays src/main.cpp ${SOURCE_FILES})
