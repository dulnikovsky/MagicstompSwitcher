cmake_minimum_required(VERSION 3.5)

project(MagicstompSwitcher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MagicstompSwitcher
    main.cpp
    magicstomp.h
    midiportidalsa.h midiportidalsa.cpp
    alsafunc.h alsafunc.cpp
    )

target_link_libraries(MagicstompSwitcher PUBLIC asound)
