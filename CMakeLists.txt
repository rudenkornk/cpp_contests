cmake_minimum_required(VERSION 3.20)

project(cpp_contests)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts)

find_package(Boost REQUIRED COMPONENTS headers)

include(CodingStandards)
include(Testing)
include(Utils)

option(CODE_COVERAGE "Enable code coverage tests" OFF)
option(LIT "Run lit tests" OFF)
option(PARALLEL_TESTING "Run tests in parallel" OFF)
option(VALGRIND "Run tests under valgrind" OFF)
option(WARNINGS "Enable strict warnings" OFF)

enable_testing()
if(CODE_COVERAGE)
  enable_code_coverage()
endif()
if(LIT)
  enable_lit_tests()
endif()
if(PARALLEL_TESTING)
  enable_parallel_testing()
endif()
if(VALGRIND)
  enable_valgrind_testing()
endif()
if(WARNINGS)
  enable_warnings()
endif()

add_subdirectory(k_max)
add_subdirectory(missing_numbers)
add_subdirectory(reverse_list)
add_subdirectory(valid_parenthesis)

if(CODE_COVERAGE)
  add_code_coverage_test()
endif()
