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
