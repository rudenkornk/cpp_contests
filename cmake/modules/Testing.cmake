include(ProcessorCount)
include(Utils)

#[[
 GLOBAL properties are meant as chosen options by user and have two types: ACTIVATED and ENABLED.
 When it is possible that not all targets support some property, then global option is a type of ACTIVATED.
 In that case it has a TARGET counterpart, which is ENABLED and indicates that the property is actually enabled for
 this specific target.
 ]]
define_property(GLOBAL PROPERTY CODE_COVERAGE_ACTIVATED)
define_property(GLOBAL PROPERTY LIT_TESTS_ENABLED)
define_property(GLOBAL PROPERTY VALGRIND_ACTIVATED)
define_property(TARGET PROPERTY CODE_COVERAGE_ENABLED)
define_property(TARGET PROPERTY VALGRIND_ENABLED)

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
    get_target_property(target_code_coverage_enabled ${target} CODE_COVERAGE_ENABLED)
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
  get_target_property(target_valgrind_enabled ${TEST_TARGET} VALGRIND_ENABLED)
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

  get_property(lit_tests_enabled GLOBAL PROPERTY LIT_TESTS_ENABLED)
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
    get_target_property(target_name ${target} OUTPUT_NAME)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      # Code coverage support. If it is not enabled, LLVM_PROFILE_FILE variable does nothing
      cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR "$(basename %s.profraw)" OUTPUT_VARIABLE llvm_profile)
      set(target_environment "LLVM_PROFILE_FILE=\\\"${llvm_profile}\\\" ")
    endif()
    get_target_property(target_valgrind_enabled ${target} VALGRIND_ENABLED)
    if(target_valgrind_enabled)
      set(valgrind_if_enabled "valgrind ")
    endif()

    string(APPEND SUBS_NAMES "\"${target_name}\", ")
    string(APPEND SUBS_PATHS "\"%(${target_name})s\", ")
    string(APPEND SUBS_RUNS "\"${target_environment}${valgrind_if_enabled}\", ")
    list(APPEND lit_options --param ${target_name}=$<TARGET_FILE:${target}>)
  endforeach()
  cmake_path(
    APPEND
    PROJECT_SOURCE_DIR
    cmake
    scripts
    strip_comments.py
    OUTPUT_VARIABLE
    strip_path)
  string(APPEND SUBS_NAMES "\"strip_comments\", ")
  string(APPEND SUBS_PATHS "\"${strip_path}\", ")
  string(APPEND SUBS_RUNS "\"python3 \", ")

  cmake_path(GET TEST_LIT_CONFIG FILENAME lit_config_name)
  string(REGEX REPLACE "\.py\.[a-zA-Z0-9_]+$" ".py" lit_config_out ${lit_config_name})
  if(DEFINED TEST_WORKING_DIRECTORY)
    cmake_path(APPEND TEST_WORKING_DIRECTORY ${lit_config_out} OUTPUT_VARIABLE lit_config_out)
  endif()
  configure_file(${TEST_LIT_CONFIG} ${lit_config_out} @ONLY)

  if("${LIT_PARALLEL}" STREQUAL "AUTO")
    ProcessorCount(PC)
    if(NOT PC EQUAL 0)
      set(LIT_PARALLEL ${PC})
    else()
      set(LIT_PARALLEL 1)
    endif()
  endif()

  list(APPEND lit_options --verbose)
  list(APPEND lit_options --workers ${LIT_PARALLEL})
  add_test(
    NAME ${TEST_NAME}
    COMMAND lit ${lit_options} .
    WORKING_DIRECTORY ${TEST_WORKING_DIRECTORY} ${TEST_UNPARSED_ARGUMENTS})
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
  set_target_properties(${TARGET} PROPERTIES CODE_COVERAGE_ENABLED ON)
endfunction()

function(_target_enable_valgrind TARGET)
  if(NOT ${ARGC} EQUAL 1)
    message(FATAL_ERROR "Provide exactly one target.")
  endif()
  set_target_properties(${TARGET} PROPERTIES VALGRIND_ENABLED ON)
endfunction()

# Enable code coverage for targets, which called target_enable_instrumentation
function(activate_code_coverage)
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

  set_property(GLOBAL PROPERTY CODE_COVERAGE_ACTIVATED ON)
endfunction()

# Run tests under valgrind for targets, which called target_enable_instrumentation
function(activate_valgrind_testing)
  find_program(valgrind valgrind REQUIRED)
  set_property(GLOBAL PROPERTY VALGRIND_ACTIVATED ON)
endfunction()

function(enable_lit_tests)
  find_package(Python3 REQUIRED)
  find_program(lit lit REQUIRED)
  find_program(FileCheck FileCheck REQUIRED)
  set_property(GLOBAL PROPERTY LIT_TESTS_ENABLED ON)
endfunction()

function(target_enable_instrumentation TARGET)
  get_property(code_coverage_activated GLOBAL PROPERTY CODE_COVERAGE_ACTIVATED)
  if(code_coverage_activated)
    _target_enable_code_coverage(${TARGET})
  endif()

  get_property(valgrind_testing_activated GLOBAL PROPERTY VALGRIND_ACTIVATED)
  if(valgrind_testing_activated)
    _target_enable_valgrind(${TARGET})
  endif()
endfunction()
