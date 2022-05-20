cmake_minimum_required(VERSION 3.20)

project(cpp_contests)

list(APPEND CMAKE_MODULE_PATH cmake)

include(CTest)
include(CodeCoverage)
include(Warnings)

option(COVERAGE "Enable code coverage tests" OFF)
option(VALGRIND "Run tests under valgrind" OFF)

add_subdirectory(k_max)
add_subdirectory(missing_numbers)
add_subdirectory(reverse_list)
add_subdirectory(valid_parenthesis)
