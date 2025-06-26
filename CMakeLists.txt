cmake_minimum_required(VERSION 3.15)

project(STI LANGUAGES C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(STI STI.c)

add_subdirectory(source)
add_subdirectory(tests)