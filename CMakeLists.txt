cmake_minimum_required(VERSION 3.8)

project(xLayoutDisplay)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)

link_libraries(${X11_LIBRARIES})
link_libraries(${X11_Xrandr_LIB})
link_libraries(${X11_Xcursor_LIB})

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
        src/Settings.cpp
        src/Settings.h
        src/util.h
        src/xrandrrutil.cpp
        src/xrandrrutil.h
        src/xrdbutil.cpp
        src/xrdbutil.h
        )

set(FILES_TST
        test/test-calculations.cpp
        test/test-Output.cpp
        test/test-Edid.cpp
        test/test-MockEdid.h
        test/test-MockMonitors.h
        test/test-Mode.cpp
        test/test-Monitors.cpp
        test/test-Settings.cpp
        test/test-xrandrutil.cpp
        test/test-xrdbutil.cpp
        )

add_executable(xLayoutDisplay ${FILES_EXE} ${FILES_SRC})

add_executable(Tests ${FILES_SRC} ${FILES_TST})

target_link_libraries(Tests gtest gmock gmock_main pthread)
