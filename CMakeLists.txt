cmake_minimum_required(VERSION 3.6)
project(xLayoutDisplays)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
link_libraries(${X11_Xrandr_LIB})

# need to discover how to link just to exe... sounds like multiple cmake files would be the way
link_libraries(gtest)

set(SOURCE_FILES
        src/xLayoutDisplays.cpp
        src/xLayoutDisplays.h
        src/laptop.cpp
        src/laptop.h
        )

set(TEST_FILES
        test/main.cpp
        test/test-laptop.cpp
        )

add_executable(gtests ${SOURCE_FILES} ${TEST_FILES})

add_executable(executable src/main.cpp ${SOURCE_FILES})
