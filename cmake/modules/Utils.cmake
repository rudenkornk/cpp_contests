function(get_targets out_var)
  # Credit: https://discourse.cmake.org/t/cmake-list-of-all-project-targets/1077/17
  set(options LIVE) # Only return non-imported non-interface targets
  set(oneValueArgs DIRECTORY)
  set(multiValueArgs BOOL_PROPERTIES)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(NOT DEFINED out_var)
    message(FATAL_ERROR "OUTPUT_VARIABLE argument is mandatory.")
  endif()
  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()
  if(NOT DEFINED ARG_DIRECTORY)
    set(ARG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
  if(ARG_LIVE)
    set(LIVE LIVE)
  endif()

  get_directory_property(all_targets DIRECTORY "${ARG_DIRECTORY}" BUILDSYSTEM_TARGETS)
  get_directory_property(subdirs DIRECTORY "${ARG_DIRECTORY}" SUBDIRECTORIES)

  foreach(subdir ${subdirs})
    get_targets(subdir_targets DIRECTORY ${subdir} ${LIVE} BOOL_PROPERTIES ${ARG_BOOL_PROPERTIES})
    list(APPEND all_targets ${subdir_targets})
  endforeach()

  foreach(target ${all_targets})
    if(ARG_LIVE)
      get_target_property(type ${target} TYPE)
      get_target_property(imported ${target} IMPORTED)
      if((${type} STREQUAL "INTERFACE_LIBRARY") OR imported)
        continue()
      endif()
    endif()
    if(ARG_BOOL_PROPERTIES)
      set(cont OFF)
      foreach(property ${ARG_BOOL_PROPERTIES})
        get_target_property(property_value ${target} ${property})
        if(NOT property_value)
          set(cont ON)
          break()
        endif()
      endforeach()
      if(cont)
        continue()
      endif()
    endif()
    list(APPEND targets ${target})
  endforeach()

  set(${out_var}
      ${targets}
      PARENT_SCOPE)
endfunction()

function(get_tests out_var)
  set(options)
  set(oneValueArgs DIRECTORY)
  set(multiValueArgs BOOL_PROPERTIES)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(NOT DEFINED out_var)
    message(FATAL_ERROR "OUTPUT_VARIABLE argument is mandatory.")
  endif()
  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()
  if(NOT DEFINED ARG_DIRECTORY)
    set(ARG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()

  get_directory_property(all_tests DIRECTORY "${ARG_DIRECTORY}" TESTS)
  get_directory_property(subdirs DIRECTORY "${ARG_DIRECTORY}" SUBDIRECTORIES)

  foreach(subdir ${subdirs})
    get_tests(subdir_tests DIRECTORY ${subdir} BOOL_PROPERTIES ${ARG_BOOL_PROPERTIES})
    list(APPEND all_tests ${subdir_tests})
  endforeach()

  foreach(test ${all_tests})
    if(ARG_BOOL_PROPERTIES)
      set(cont OFF)
      foreach(property ${ARG_BOOL_PROPERTIES})
        get_test_property(property_value ${test} ${property})
        if(NOT property_value)
          set(cont ON)
          break()
        endif()
      endforeach()
      if(cont)
        continue()
      endif()
    endif()
    list(APPEND tests ${test})
  endforeach()

  set(${out_var}
      ${tests}
      PARENT_SCOPE)
endfunction()

# Normally we do not add sources of one target to another target
#
# But this might be useful in two cases:
#
# 1. A closely related target, such as command-line wrapper for library
# 2. Unit test, testing specifically this library
function(target_add_headers_as_sources TARGET FROM)
  get_target_property(header_sets ${FROM} INTERFACE_HEADER_SETS)
  foreach(header_set ${header_sets})
    get_target_property(header_files ${FROM} HEADER_SET_${header_set})
    target_sources(${TARGET} PRIVATE ${header_files})
  endforeach()
endfunction()

# Helper for HEADER_SET where there is multiple possible base directories for a certain header
function(get_relative_path out_var)
  set(options)
  set(oneValueArgs PATH)
  set(multiValueArgs BASE_DIRS)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(NOT DEFINED out_var)
    message(FATAL_ERROR "OUTPUT_VARIABLE argument is mandatory.")
  endif()
  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()

  foreach(base_dir ${ARG_BASE_DIRS})
    cmake_path(IS_PREFIX base_dir "${ARG_PATH}" result)
    if(result)
      cmake_path(RELATIVE_PATH ARG_PATH BASE_DIRECTORY "${base_dir}")
      cmake_path(NORMAL_PATH ARG_PATH)
      set(${out_var}
          ${ARG_PATH}
          PARENT_SCOPE)
      return()
    endif()
  endforeach()
  message(FATAL_ERROR "Cannot find appropriate BASE_DIR from (${ARG_BASE_DIRS}) for a given PATH (${ARG_PATH}).")
endfunction()
