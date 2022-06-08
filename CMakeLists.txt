cmake_minimum_required(VERSION 3.20)

project(cpp_contests)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts)

include(Utils)
include(Testing)
include(Warnings)

option(WARNINGS "Enable strict warnings" OFF)
option(CODE_COVERAGE "Enable code coverage tests" OFF)
option(LIT "Run lit tests" OFF)
option(PARALLEL_TESTING "Run tests in parallel" OFF)
option(VALGRIND "Run tests under valgrind" OFF)

enable_testing()
if(PARALLEL_TESTING)
  enable_parallel_testing()
endif()
if(VALGRIND)
  enable_valgrind_testing()
endif()
if(LIT)
  enable_lit_tests()
endif()

add_subdirectory(code_coverage_test)
add_subdirectory(k_max)
add_subdirectory(missing_numbers)
add_subdirectory(reverse_list)
add_subdirectory(valid_parenthesis)

if(WARNINGS)
  enable_warnings()
endif()
if(CODE_COVERAGE)
  enable_code_coverage()
endif()
