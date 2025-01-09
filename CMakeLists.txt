cmake_minimum_required(VERSION 3.5)

project(MagicstompSwitcher LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(WITH_QT_GUI TRUE CACHE BOOL "Determinates build with or without Qt Widgets GUI")
SET(WITH_SSD1306_DISPLAY TRUE CACHE BOOL "Determinates build with or without SSD 1306 OLED display support")

if(${WITH_QT_GUI} STREQUAL ON)
    message("Building with Qt GUI")
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
    add_compile_definitions(WITH_QT_GUI)
else()
    message("Building without Qt GUI")
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
endif()

if(${WITH_SSD1306_DISPLAY} STREQUAL ON)
    add_compile_definitions(WITH_SSD1306_DISPLAY)
endif()

set(PROJECT_SOURCES
    main.cpp
    magicstomp.h
    midiportidalsa.h
    midiportidalsa.cpp
    alsafunc.h
    alsafunc.cpp
    msswitcherthread.h
    msswitcherthread.cpp
    msswitchersettings.h
    msswitchersettings.cpp
)

if(${WITH_QT_GUI} STREQUAL ON)
    list (APPEND PROJECT_SOURCES
        mainwidget.h
        mainwidget.cpp
        currentpatchesmodel.h
        currentpatchesmodel.cpp
        delayclickablelabel.h
        delayclickablelabel.cpp
        preferencesdialog.h
        preferencesdialog.cpp
        aconnectwidget.h
        aconnectwidget.cpp
    )
endif()

if(${WITH_SSD1306_DISPLAY} STREQUAL ON)
    list (APPEND PROJECT_SOURCES
        ssd1306_display.h
        ssd1306_display.cpp
        gpiohandlerthread.h
        gpiohandlerthread.cpp
    )
endif()

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

if(${WITH_QT_GUI} STREQUAL ON)
    target_link_libraries(MagicstompSwitcher PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
else()
    target_link_libraries(MagicstompSwitcher PRIVATE Qt${QT_VERSION_MAJOR}::Core)
endif()

if(${WITH_SSD1306_DISPLAY} STREQUAL ON)
    target_link_libraries(MagicstompSwitcher PRIVATE ssd1306_i2c)
endif()

target_link_libraries(MagicstompSwitcher PRIVATE asound)
