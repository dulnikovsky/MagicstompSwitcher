cmake_minimum_required(VERSION 3.5)

project(MagicstompSwitcher LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    magicstomp.h
    midiportidalsa.h
    midiportidalsa.cpp
    alsafunc.h
    alsafunc.cpp
    msswitcherthread.h
    msswitcherthread.cpp
    mainwidget.h
    mainwidget.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MagicstompSwitcher
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(MagicstompSwitcher SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(MagicstompSwitcher
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MagicstompSwitcher PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(MagicstompSwitcher PRIVATE asound)
