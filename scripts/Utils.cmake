function(get_all_targets out_var)
  # Credit:
  # https://discourse.cmake.org/t/cmake-list-of-all-project-targets/1077/17
  if(NOT DEFINED ARGV1)
    set(current_dir ${CMAKE_SOURCE_DIR})
  else()
    set(current_dir ${ARGV1})
  endif()
  get_property(
    targets
    DIRECTORY ${current_dir}
    PROPERTY BUILDSYSTEM_TARGETS)
  get_property(
    subdirs
    DIRECTORY ${current_dir}
    PROPERTY SUBDIRECTORIES)

  foreach(subdir ${subdirs})
    get_all_targets(subdir_targets ${subdir})
    list(APPEND targets ${subdir_targets})
  endforeach()

  set(${out_var}
      ${targets}
      PARENT_SCOPE)
endfunction()

function(get_all_tests out_var)
  if(NOT DEFINED ARGV1)
    set(current_dir ${CMAKE_SOURCE_DIR})
  else()
    set(current_dir ${ARGV1})
  endif()
  get_property(
    tests
    DIRECTORY ${current_dir}
    PROPERTY TESTS)
  get_property(
    subdirs
    DIRECTORY ${current_dir}
    PROPERTY SUBDIRECTORIES)

  foreach(subdir ${subdirs})
    get_all_tests(subdir_tests ${subdir})
    list(APPEND tests ${subdir_tests})
  endforeach()

  set(${out_var}
      ${tests}
      PARENT_SCOPE)
endfunction()

# Normally we do not add sources of one target to another target
# But this might be useful in two cases:
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
