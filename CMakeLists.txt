cmake_minimum_required(VERSION 3.23)

project(
  cpp_contests
  VERSION 0.1.0
  DESCRIPTION "One-file solutions for C++ problems"
  HOMEPAGE_URL https://github.com/rudenkornk/cpp_contests
  )

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts)

find_package(Boost REQUIRED COMPONENTS headers)

include(CodingStandards)
include(GNUInstallDirs)
include(Installing)
include(Testing)
include(Utils)

option(ASAN "Enable address sanitizer" OFF)
option(CLANG_TIDY "Enable clang-tidy" OFF)
option(CODE_COVERAGE "Enable code coverage tests" OFF)
option(LIT "Run lit tests" OFF)
option(PARALLEL_TESTING "Run tests in parallel" OFF)
option(TSAN "Enable thread sanitizer" OFF)
option(UBSAN "Enable undefined behaviour sanitizer" OFF)
option(VALGRIND "Run tests under valgrind" OFF)
option(WARNINGS "Enable strict warnings" OFF)

enable_testing()
if(ASAN)
  activate_address_sanitizer()
endif()
if(CLANG_TIDY)
  activate_clang_tidy()
endif()
if(CODE_COVERAGE)
  activate_code_coverage()
endif()
if(LIT)
  enable_lit_tests()
endif()
if(PARALLEL_TESTING)
  enable_parallel_testing()
endif()
if(TSAN)
  activate_thread_sanitizer()
endif()
if(UBSAN)
  activate_ub_sanitizer()
endif()
if(VALGRIND)
  activate_valgrind_testing()
endif()
if(WARNINGS)
  activate_warnings()
endif()

add_subdirectory(k_max)
add_subdirectory(missing_numbers)
add_subdirectory(reverse_list)
add_subdirectory(valid_parenthesis)

if(CODE_COVERAGE)
  add_code_coverage_test(THRESHOLD 80)
endif()

generic_install()
configure_test_install_project()
configure_cpack_options(EMAIL "rudenkornk@gmail.com")

include(CPack)
