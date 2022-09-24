cmake_minimum_required(VERSION 3.19)

include(CheckSymbolExists)

project(libsys VERSION 0.1 LANGUAGES CXX)

option(LIBSYS_TESTS "Build the tests" OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)

check_symbol_exists(fdatasync "unistd.h"     HAVE_FDATASYNC)
check_symbol_exists(pread     "unistd.h"     HAVE_PREADWRITE)
check_symbol_exists(pselect   "sys/select.h" HAVE_PSELECT)
check_symbol_exists(ppoll     "poll.h"       HAVE_PPOLL)

configure_file(config.h.in config.h)

add_library(sys INTERFACE)

target_include_directories(sys INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_compile_features(sys INTERFACE cxx_std_17)

if(LIBSYS_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
