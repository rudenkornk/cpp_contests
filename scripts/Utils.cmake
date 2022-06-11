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
      set(contiue OFF)
      foreach(property ${ARG_BOOL_PROPERTIES})
        get_target_property(property_value ${target} ${property})
        if(NOT property_value)
          set(continue ON)
          break()
        endif()
      endforeach()
      if(continue)
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
      set(continue OFF)
      foreach(property ${ARG_BOOL_PROPERTIES})
        get_test_property(property_value ${test} ${property})
        if(NOT property_value)
          set(continue ON)
          break()
        endif()
      endforeach()
      if(continue)
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
  get_target_property(includes ${FROM} INTERFACE_INCLUDE_DIRECTORIES)
  foreach(dir ${includes})
    file(GLOB tmp CONFIGURE_DEPENDS ${dir}/*.hpp)
    set(headers ${headers} ${tmp})
  endforeach()
  target_sources(${TARGET} PRIVATE ${headers})
endfunction()
