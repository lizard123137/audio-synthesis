cmake_minimum_required(VERSION 3.20)
project(AudioSynth LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/miniaudio.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    PkgConfig::GTK4
)