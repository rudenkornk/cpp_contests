cmake_minimum_required(VERSION 3.23)

project(
  cpp_contests
  VERSION 0.1.0
  DESCRIPTION "Repository with solutions for different C++ educational problems."
  HOMEPAGE_URL https://github.com/rudenkornk/cpp_contests)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(fmt CONFIG REQUIRED)

include(CodingStandards)
include(Installing)
include(Testing)
include(Utils)

option(BUILD_SHARED_LIBS "Build shared libs instead of static ones" OFF)
option(CODE_COVERAGE "Enable code coverage tests" OFF)
option(LINT "Enable lint tests" OFF)
option(LIT "Enable lit tests" OFF)
set(LIT_PARALLEL
    1
    CACHE STRING "Parallel level for LIT tests")
option(VALGRIND "Run tests under valgrind" OFF)

enable_testing()
add_format_target()
if(LINT)
  add_lint_test()
  activate_clang_tidy()
endif()
if(CODE_COVERAGE)
  activate_code_coverage()
endif()
if(LIT)
  enable_lit_tests()
endif()
if(VALGRIND)
  activate_valgrind_testing()
endif()
activate_warnings()

add_subdirectory(src)

if(CODE_COVERAGE)
  add_code_coverage_test(THRESHOLD 5)
endif()

generic_install()
configure_test_install_project(FIND_PACKAGES fmt VCPKG_DEPS fmt)
configure_cpack_options(EMAIL "rudenkornk@gmail.com")

include(CPack)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_BINARY_DIR}/compile_commands.json
                        ${PROJECT_SOURCE_DIR}/compile_commands.json)
