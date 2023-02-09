cmake_minimum_required(VERSION 3.10)

project(Legion-Y720-Keyboard-Backligh C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(kbd-backlight ${SOURCES})
target_link_libraries(kbd-backlight config)
target_include_directories(kbd-backlight PRIVATE src/)
