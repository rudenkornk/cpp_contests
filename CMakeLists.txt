cmake_minimum_required(VERSION 4.0)

project(
  cpp_contests
  VERSION 0.1.0
  DESCRIPTION "Repository with solutions for different C++ educational problems."
  HOMEPAGE_URL https://github.com/rudenkornk/cpp_contests)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

option(BUILD_SHARED_LIBS "Build shared libs instead of static ones." OFF)
option(CODE_COVERAGE "Enable code coverage tests." OFF)
option(LINT "Enable lint tests." OFF)
option(WARNINGS "Enable strict warnings." ON)

find_package(Boost REQUIRED COMPONENTS program_options unit_test_framework)

include(CodingStandards)
include(Installing)
include(Testing)
include(Utils)

enable_testing()
add_format_target()
if(LINT)
  add_lint_test()
  activate_clang_tidy()
endif()
if(CODE_COVERAGE)
  enable_code_coverage()
endif()
if(WARNINGS)
  activate_warnings()
endif()

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  # See https://gitlab.kitware.com/cmake/cmake/-/issues/20912#note_795673
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_BINARY_DIR}/compile_commands.json
                          ${PROJECT_SOURCE_DIR}/compile_commands.json)
endif()

# Workaround for the Clang toolchain shipped by nixpkgs 26.05 (Clang 21 paired with the gcc 15 libstdc++). clang-tidy
# parses each module in a driver mode whose `-fstrict-flex-arrays` default differs from the value clang++ baked into the
# precompiled `.pcm`, so every module fails to load with a configuration mismatch. Pin the flag to keep the module build
# and clang-tidy in agreement. The generator expression keeps it Clang-only, so the gcc presets and non-Clang consumers
# of the installed modules never see it.
add_compile_options($<$<CXX_COMPILER_ID:Clang>:-fstrict-flex-arrays=0>)

add_subdirectory(src)

if(CODE_COVERAGE)
  add_code_coverage_test(THRESHOLD 70)
endif()

generic_install()
configure_test_install_project(FIND_PACKAGES Boost)
configure_cpack_options(EMAIL "rudenkornk@gmail.com")

include(CPack)
