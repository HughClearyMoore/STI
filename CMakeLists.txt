cmake_minimum_required(VERSION 3.15)

project(STI LANGUAGES C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(STI_BUILD_TESTS "Build the STI test suite" ON)

add_library(STI STI.c)

add_subdirectory(source)

target_include_directories(STI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source/include")

if(STI_BUILD_TESTS)
    add_subdirectory(tests)
endif()