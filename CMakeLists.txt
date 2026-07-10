##############################################################
# Bookggle PocketBook build (multi-file).
#
#   export PBSDK=/path/to/SDK_6.3.0
#   mkdir build && cd build
#   cmake .. -DCMAKE_TOOLCHAIN_FILE=$PBSDK/share/cmake/arm_conf.cmake
#   make
#
# All src/*.c modules are picked up automatically via file(GLOB) —
# add a new .c file and it's built, no edit needed here.
##############################################################

cmake_minimum_required(VERSION 3.10)
project(bookggle C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT DEFINED ENV{PBSDK} OR "$ENV{PBSDK}" STREQUAL "")
    message(FATAL_ERROR "Please export PBSDK=/path/to/SDK_6.3.0 before configuring this project.")
endif()

if(NOT EXISTS "$ENV{PBSDK}/share/cmake/arm_conf.cmake")
    message(FATAL_ERROR "PocketBook toolchain file not found at $ENV{PBSDK}/share/cmake/arm_conf.cmake")
endif()

include_directories(
    src/include
    $ENV{PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/local/include
)

link_directories(
    $ENV{PBSDK}/arm-obreey-linux-gnueabi/sysroot/usr/local/lib
)

file(GLOB BOOKGGLE_SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(bookggle.app ${BOOKGGLE_SOURCES})

target_link_libraries(
    bookggle.app
    inkview
    m
)

target_compile_options(
    bookggle.app PRIVATE
    -Wall -Wextra -O2
)
