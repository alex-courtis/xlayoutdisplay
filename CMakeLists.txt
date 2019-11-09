cmake_minimum_required(VERSION 3.8)

project(xlayoutdisplay)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_DEBUG_LIBS OFF)
set(Boost_USE_STATIC_LIBS ON)

find_package(X11 REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

link_libraries(${X11_LIBRARIES})
link_libraries(${X11_Xrandr_LIB})
link_libraries(${X11_Xcursor_LIB})
link_libraries(${Boost_LIBRARIES})

set(FILES_EXE
        main.cpp
        )

set(FILES_SRC
        src/calculations.cpp
        src/calculations.h
        src/Output.cpp
        src/Output.h
        src/Edid.cpp
        src/Edid.h
        src/layout.cpp
        src/layout.h
        src/Mode.cpp
        src/Mode.h
        src/Monitors.cpp
        src/Monitors.h
        src/Pos.h
        src/Settings.h
        src/util.h
        src/xrandrrutil.cpp
        src/xrandrrutil.h
        src/xrdbutil.cpp
        src/xrdbutil.h
        src/xutil.cpp
        src/xutil.h
        )

set(FILES_TST
        test/test-calculations.cpp
        test/test-Output.cpp
        test/test-Edid.cpp
        test/test-MockEdid.h
        test/test-MockMonitors.h
        test/test-Mode.cpp
        test/test-Monitors.cpp
        test/test-xrandrutil.cpp
        test/test-xrdbutil.cpp
        )

add_executable(xlayoutdisplay
        ${FILES_EXE} ${FILES_SRC}
        )

install(TARGETS xlayoutdisplay
        RUNTIME DESTINATION bin)


find_package(GTest)

find_library(GMOCK_FOUND gmock)

if (GTEST_FOUND AND GMOCK_FOUND)
    enable_testing()

    add_executable(unitTests
            ${FILES_SRC} ${FILES_TST}
            )

    target_link_libraries(unitTests
            gtest gmock gmock_main pthread
            )

    gtest_discover_tests(unitTests)
endif()
