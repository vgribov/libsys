cmake_minimum_required(VERSION 3.19)

include(CheckSymbolExists)

project(hello VERSION 0.1 LANGUAGES CXX)

option(PACKAGE_TESTS "Build the tests" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)
include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

check_symbol_exists(fdatasync "unistd.h"     HAVE_FDATASYNC)
check_symbol_exists(pread     "unistd.h"     HAVE_PREADWRITE)
check_symbol_exists(pselect   "sys/select.h" HAVE_PSELECT)
check_symbol_exists(ppoll     "poll.h"       HAVE_PPOLL)

configure_file(config.h.in config.h)

if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
