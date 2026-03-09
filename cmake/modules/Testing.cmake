include(ProcessorCount)
include(Utils)

define_property(GLOBAL PROPERTY CODE_COVERAGE_ENABLED)
define_property(TARGET PROPERTY CODE_COVERAGE_ALLOWED)

function(add_code_coverage_test)
  set(options)
  set(oneValueArgs THRESHOLD)
  set(multiValueArgs)
  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()
  if(NOT DEFINED ARG_THRESHOLD)
    message(FATAL_ERROR "THRESHOLD is required argument.")
  endif()

  get_targets(targets LIVE)
  get_tests(tests)
  foreach(target ${targets})
    get_target_property(target_code_coverage_enabled ${target} CODE_COVERAGE_ALLOWED)
    if(target_code_coverage_enabled)
      list(APPEND code_coveraged_targets ${target})
    endif()
  endforeach()
  if(NOT code_coveraged_targets)
    message(WARNING "No targets with coded coverage defined.\nCode coverage tests will not be added.")
    return()
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_test(
      NAME merge_code_coverage_data
      COMMAND bash -c "llvm-profdata merge --sparse --output profdata $(find -name *.profraw)"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    set_tests_properties(merge_code_coverage_data PROPERTIES DEPENDS "${tests}" FIXTURES_SETUP merge_code_coverage_data)

    foreach(target ${code_coveraged_targets})
      string(APPEND objects "-object $<TARGET_FILE:${target}> ")
    endforeach()
    add_test(
      NAME code_coverage_report
      COMMAND bash -c
              "llvm-cov report --instr-profile profdata ${objects} | tee ${PROJECT_BINARY_DIR}/code_coverage_report"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    set_tests_properties(code_coverage_report PROPERTIES FIXTURES_REQUIRED merge_code_coverage_data)
    set_tests_properties(code_coverage_report PROPERTIES FIXTURES_SETUP code_coverage_report)

    add_test(
      NAME code_coverage_check
      COMMAND
        bash -c "(( \
        $( \
        grep --only-matching --perl-regexp \
        \"TOTAL(\\s+\\d+(\\.\\d+)?%?){8}\\s+\\K\\d+\" \
        ${PROJECT_BINARY_DIR}/code_coverage_report\
        ) >= ${ARG_THRESHOLD} ))"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    set_tests_properties(code_coverage_check PROPERTIES FIXTURES_REQUIRED code_coverage_report)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # also can be done with lcov: lcov --directory ${PROJECT_BINARY_DIR} --capture --output-file
    # ${PROJECT_BINARY_DIR}/coverage_report/coverage.info
    #
    # lcov --summary ${PROJECT_BINARY_DIR}/coverage_report/coverage.info
    add_test(
      NAME code_coverage_report
      COMMAND
        bash -c
        "gcovr ${PROJECT_BINARY_DIR} --exclude ${PROJECT_BINARY_DIR} | tee ${PROJECT_BINARY_DIR}/code_coverage_report"
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    set_tests_properties(code_coverage_report PROPERTIES DEPENDS "${tests}")
    set_tests_properties(code_coverage_report PROPERTIES FIXTURES_SETUP code_coverage_report)

    add_test(
      NAME code_coverage_check
      COMMAND
        bash -c "(( \
        $(\
        grep --only-matching --perl-regexp \
        \"TOTAL(\\s+\\d+){2}\\s+\\K\\d+\" \
        ${PROJECT_BINARY_DIR}/code_coverage_report\
        ) >= ${ARG_THRESHOLD} ))"
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
    set_tests_properties(code_coverage_check PROPERTIES FIXTURES_REQUIRED code_coverage_report)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(WARNING "Code coverage test not implemented for MSVC.")
  endif()
endfunction()

function(add_tests)
  set(options)
  set(oneValueArgs TARGET)
  set(multiValueArgs COMMAND_ARGUMENTS)
  cmake_parse_arguments(PARSE_ARGV 0 TEST "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(NOT DEFINED TEST_TARGET)
    message(FATAL_ERROR "TARGET is required argument.")
  endif()
  if(DEFINED TEST_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()

  if(CMAKE_TEST_LAUNCHER)
    list(GET CMAKE_TEST_LAUNCHER 0 launcher)
    find_program(_ REQUIRED NAMES ${launcher})

    list(JOIN CMAKE_TEST_LAUNCHER " " shell_test_launcher)
    string(APPEND shell_test_launcher " ")
  endif()

  # Inspect dependencies and add executable targets as CLI arguments.
  get_target_property(dependencies ${TEST_TARGET} MANUALLY_ADDED_DEPENDENCIES)
  if(dependencies)
    list(APPEND TEST_COMMAND_ARGUMENTS "--")

    foreach(dep IN LISTS dependencies)
      if(TARGET ${dep})
        get_target_property(dep_type ${dep} TYPE)
        if(dep_type STREQUAL "EXECUTABLE")
          list(APPEND TEST_COMMAND_ARGUMENTS "--${dep}=${shell_test_launcher}$<TARGET_FILE:${dep}>")
        endif()
      endif()
    endforeach()
  endif()

  add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET} ${TEST_COMMAND_ARGUMENTS})

  get_property(code_coverage_enabled GLOBAL PROPERTY CODE_COVERAGE_ENABLED)
  get_target_property(target_code_coverage_allowed ${TEST_TARGET} CODE_COVERAGE_ALLOWED)

  if(code_coverage_enabled AND target_code_coverage_allowed)
    cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR "%p.profraw" OUTPUT_VARIABLE llvm_profile)
    set_tests_properties(${TEST_TARGET} PROPERTIES ENVIRONMENT "LLVM_PROFILE_FILE=${llvm_profile}")
  endif()
endfunction()

function(_target_enable_code_coverage TARGET)
  if(NOT ${ARGC} EQUAL 1)
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
    message(FATAL_ERROR "Code coverage not implemented for MSVC")
  endif()
  set_target_properties(${TARGET} PROPERTIES CODE_COVERAGE_ALLOWED ON)
endfunction()

# Enable code coverage for targets, which called target_allow_instrumentation.
function(enable_code_coverage)
  find_package(Python3 REQUIRED)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    find_program(llvm_profdata llvm-profdata REQUIRED)
    find_program(llvm_cov llvm-cov REQUIRED)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_program(gcovr gcovr REQUIRED)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    string(APPEND message "Code coverage tests are not implemented for MSVC.")
    string(APPEND message "\nCode coverage tests will not be enabled.")
    message(FATAL_ERROR ${message})
    return()
  endif()

  set_property(GLOBAL PROPERTY CODE_COVERAGE_ENABLED ON)
endfunction()

function(target_allow_instrumentation TARGET)
  get_property(code_coverage_activated GLOBAL PROPERTY CODE_COVERAGE_ENABLED)
  if(code_coverage_activated)
    _target_enable_code_coverage(${TARGET})
  endif()
endfunction()
