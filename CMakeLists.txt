cmake_minimum_required(VERSION 3.6)
project(xLayoutDisplays)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
link_libraries(${X11_Xrandr_LIB})

# need to discover how to link just to exe... sounds like multiple cmake files would be the way
link_libraries(gtest)

set(SOURCE_FILES
        src/Displ.cpp
        src/Displ.h
        src/Mode.cpp
        src/Mode.h
        src/Pos.h
        src/laptop.cpp
        src/laptop.h
        src/layout.cpp
        src/layout.h
        src/xLayoutDisplays.cpp
        src/xLayoutDisplays.h
        src/xrandrrutil.c
        src/xrandrrutil.h
        )

set(TEST_FILES
        test/main.cpp
        test/test-Displ.cpp
        test/test-laptop.cpp
        test/test-layout.cpp
        test/test-Mode.cpp
        test/test-xrandrutil.cpp
        )

add_executable(gtests ${SOURCE_FILES} ${TEST_FILES})

add_executable(xLayoutDisplays src/main.cpp ${SOURCE_FILES})
