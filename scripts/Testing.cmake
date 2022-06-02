include(ProcessorCount)
include(Utils)

define_property(TARGET PROPERTY CODE_COVERAGE_ENABLED)
define_property(TARGET PROPERTY VALGRIND_ENABLED)

function(enable_parallel_testing)
  if(${ARGC} GREATER 1)
    message(FATAL_ERROR "This function accepts at most 1 argument.")
  endif()
  if(${ARGC} EQUAL 1)
    set(test_workers
        ${ARGV0}
        PARENT_SCOPE)
  else()
    ProcessorCount(PC)
    if(NOT PC EQUAL 0)
      set(test_workers
          ${PC}
          PARENT_SCOPE)
    endif()
  endif()
  set(CTEST_BUILD_FLAGS
      -j${test_workers}
      PARENT_SCOPE)
endfunction()

function(add_code_coverage_test)
  get_all_targets(targets)
  get_all_tests(tests)
  foreach(target ${targets})
    get_property(
      target_code_coverage_enabled
      TARGET ${target}
      PROPERTY CODE_COVERAGE_ENABLED)
    if(target_code_coverage_enabled)
      list(APPEND code_coveraged_targets ${target})
    endif()
  endforeach()
  if(NOT code_coveraged_targets)
    message(FATAL_ERROR "No targets with coded coverage defined!")
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_test(
      NAME merge_code_coverage_data
      COMMAND bash -c "llvm-profdata merge --output profdata $(find -name *.profraw)"
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    set_tests_properties(merge_code_coverage_data PROPERTIES DEPENDS "${tests}")
    foreach(target ${code_coveraged_targets})
      list(APPEND objects -object $<TARGET_FILE:${target}>)
    endforeach()
    add_test(
      NAME code_coverage_report
      COMMAND llvm-cov report --instr-profile profdata ${objects}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    set_tests_properties(code_coverage_report PROPERTIES DEPENDS merge_code_coverage_data)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # also can be done with lcov: lcov --directory ${CMAKE_BINARY_DIR} --capture --output-file
    # ${CMAKE_BINARY_DIR}/coverage_report/coverage.info lcov --summary ${CMAKE_BINARY_DIR}/coverage_report/coverage.info
    add_test(
      NAME code_coverage_report
      COMMAND gcovr ${CMAKE_BINARY_DIR}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    set_tests_properties(code_coverage_report PROPERTIES DEPENDS "${tests}")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(WARNING "Code coverage test not implemented for MSVC.")
  endif()
endfunction()

function(add_unit_tests)
  set(options)
  set(oneValueArgs NAME TARGET)
  set(multiValueArgs COMMAND_ARGUMENTS)
  cmake_parse_arguments(PARSE_ARGV 0 TEST "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(NOT DEFINED TEST_NAME)
    message(FATAL_ERROR "NAME is required argument.")
  endif()
  if(NOT DEFINED TEST_TARGET)
    message(FATAL_ERROR "TARGET is required argument.")
  endif()
  get_property(
    target_valgrind_enabled
    TARGET ${TEST_TARGET}
    PROPERTY VALGRIND)
  if(target_valgrind_enabled)
    add_test(NAME ${TEST_NAME} COMMAND valgrind $<TARGET_FILE:${TEST_TARGET}> ${TEST_COMMAND_ARGUMENTS}
                                       ${TEST_UNPARSED_ARGUMENTS})
  else()
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_TARGET} ${TEST_COMMAND_ARGUMENTS} ${TEST_UNPARSED_ARGUMENTS})
  endif()
endfunction()

function(add_lit_tests)
  set(options)
  set(oneValueArgs NAME WORKING_DIRECTORY LIT_CONFIG)
  set(multiValueArgs COMMAND TARGETS)
  cmake_parse_arguments(PARSE_ARGV 0 TEST "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(NOT lit_tests_enabled)
    return()
  endif()

  if(NOT DEFINED TEST_NAME)
    message(FATAL_ERROR "NAME is required argument.")
  endif()
  if(NOT DEFINED TEST_TARGETS)
    message(FATAL_ERROR "TARGETS is required argument.")
  endif()
  if(NOT DEFINED TEST_LIT_CONFIG)
    message(FATAL_ERROR "LIT_CONFIG is required argument.")
  endif()
  if(DEFINED TEST_COMMAND)
    message(FATAL_ERROR "COMMAND is not allowed here.")
  endif()

  foreach(target ${TEST_TARGETS})
    get_property(
      target_name
      TARGET ${target}
      PROPERTY OUTPUT_NAME)
    get_property(
      target_code_coverage_enabled
      TARGET ${target}
      PROPERTY CODE_COVERAGE_ENABLED)
    if((target_code_coverage_enabled) AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
      cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR "$(basename %s.profraw)" OUTPUT_VARIABLE llvm_profile)
      set(target_environment "LLVM_PROFILE_FILE=\\\"${llvm_profile}\\\" ")
    endif()
    get_property(
      target_valgrind_enabled
      TARGET ${target}
      PROPERTY VALGRIND)
    if(target_valgrind_enabled)
      set(valgrind_if_enabled "valgrind ")
    endif()

    string(APPEND SUBS_NAMES "\"${target_name}\", ")
    string(APPEND SUBS_PATHS "\"%(${target_name})s\", ")
    string(APPEND SUBS_RUNS "\"${target_environment}${valgrind_if_enabled}\", ")
    list(APPEND lit_options --param ${target_name}=$<TARGET_FILE:${target}>)
  endforeach()
  cmake_path(APPEND CMAKE_SOURCE_DIR scripts strip_comments.py OUTPUT_VARIABLE strip_path)
  string(APPEND SUBS_NAMES "\"strip_comments\", ")
  string(APPEND SUBS_PATHS "\"${strip_path}\", ")
  string(APPEND SUBS_RUNS "\"python3 \", ")

  cmake_path(GET TEST_LIT_CONFIG FILENAME lit_config_name)
  string(REGEX REPLACE "\.py\.[a-zA-Z0-9_]+$" ".py" lit_config_out ${lit_config_name})
  if(DEFINED TEST_WORKING_DIRECTORY)
    cmake_path(APPEND TEST_WORKING_DIRECTORY ${lit_config_out} OUTPUT_VARIABLE lit_config_out)
  endif()
  configure_file(${TEST_LIT_CONFIG} ${lit_config_out} @ONLY)

  list(APPEND lit_options --verbose)
  if(DEFINED test_workers)
    list(APPEND lit_options --workers ${test_workers})
  endif()
  add_test(
    NAME ${TEST_NAME}
    COMMAND lit ${lit_options} .
    WORKING_DIRECTORY ${TEST_WORKING_DIRECTORY} ${TEST_UNPARSED_ARGUMENTS})
endfunction()

function(target_enable_code_coverage TARGET)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "Provide exactly one target.")
  endif()
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${TARGET} PRIVATE -fprofile-instr-generate -fcoverage-mapping -mllvm
                                             -enable-name-compression=false)
    target_link_options(${TARGET} PRIVATE -fprofile-instr-generate)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${TARGET} PRIVATE --coverage)
    target_link_libraries(${TARGET} PRIVATE gcov)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${TARGET} PRIVATE /fsanitize-coverage)
  endif()
  set_property(TARGET ${TARGET} PROPERTY CODE_COVERAGE_ENABLED ON)
endfunction()

function(target_enable_valgrind TARGET)
  if(${ARGC} LESS 1)
    message(FATAL_ERROR "Provide exactly one target.")
  endif()
  set_property(TARGET ${TARGET} PROPERTY VALGRIND ON)
endfunction()

function(enable_code_coverage)
  set(code_coverage_enabled
      ON
      PARENT_SCOPE)
endfunction()

function(enable_valgrind_testing)
  find_program(valgrind valgrind REQUIRED)
  set(valgrind_testing_enabled
      ON
      PARENT_SCOPE)
endfunction()

function(enable_lit_tests)
  find_package(Python3 REQUIRED)
  find_program(lit lit REQUIRED)
  find_program(filecheck FileCheck REQUIRED)
  set(lit_tests_enabled
      ON
      PARENT_SCOPE)
endfunction()

function(target_enable_instrumentation TARGET)
  if(code_coverage_enabled)
    target_enable_code_coverage(${TARGET})
  endif()
  if(valgrind_testing_enabled)
    target_enable_valgrind(${TARGET})
  endif()
endfunction()
