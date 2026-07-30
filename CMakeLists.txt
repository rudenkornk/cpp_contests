cmake_minimum_required(VERSION 4.0)

# Opt into CMake's still-experimental `import std;` support. The gate is keyed by a UUID that changes with every CMake
# release, so a nixpkgs bump that moves CMake will need the new value (CMake prints it in a fatal error when the token
# goes stale). This must be set before the CXX language is enabled, i.e. before `project()`. The current value matches
# the CMake 4.3 shipped by the flake.
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")

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

# Build the standard library module so translation units can `import std;` instead of textually including headers.
# Combined with the experimental gate above, this makes CMake compile libstdc++'s `std`/`std.compat` module and attach
# it to every C++23 target automatically.
set(CMAKE_CXX_MODULE_STD ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # CMake normally locates the libstdc++ `import std` manifest through `clang++
  # -print-file-name=libstdc++.modules.json`, but the nixpkgs clang wrapper supplies libstdc++ via wrapper flags, so
  # that query returns nothing. The devshell exports the manifest path (and the `backward/` header directory that g++
  # adds implicitly but the clang wrapper does not; libstdc++'s `std.cc` includes `<strstream>` from there) for the
  # exact gcc this clang compiles against.
  set(CMAKE_CXX_STDLIB_MODULES_JSON "$ENV{IMPORT_STD_MANIFEST}")
  list(APPEND CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES "$ENV{IMPORT_STD_BACKWARD_INC}")
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
