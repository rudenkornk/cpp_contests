include(Utils)

define_property(GLOBAL PROPERTY CLANG_TIDY_ACTIVATED)
define_property(GLOBAL PROPERTY WARNINGS_ACTIVATED)
define_property(TARGET PROPERTY CLANG_TIDY_ENABLED)
define_property(TARGET PROPERTY WARNINGS_ENABLED)

function(target_enable_warnings TARGET)
  if(NOT ${ARGC} EQUAL 1)
    message(FATAL_ERROR "Provide exactly one target.")
  endif()
  if((CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -pedantic -Werror)
  elseif((CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"))
    target_compile_options(${TARGET} PRIVATE /W4 /WX)
  else()
    message(
      WARNING
        "Compiler with id \"${CMAKE_CXX_COMPILER_ID}\" and variant \"${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}\" is not implemented here. Warnings will not be enabled."
    )
  endif()
  set_target_properties(${TARGET} PROPERTIES WARNINGS_ENABLED ON)
endfunction()

function(target_disable_warnings TARGET)
  if(NOT ${ARGC} EQUAL 1)
    message(FATAL_ERROR "Provide exactly one target.")
  endif()
  if((CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    get_target_property(target_options ${TARGET} COMPILE_OPTIONS)
    list(REMOVE_ITEM target_options -Wall -Wextra -pedantic -Werror)
    list(APPEND target_options -w)
    set_target_properties(${TARGET} PROPERTIES COMPILE_OPTIONS ${target_options})
  elseif((CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"))
    get_target_property(target_options ${TARGET} COMPILE_OPTIONS)
    list(REMOVE_ITEM target_options /W4 /WX)
    list(APPEND target_options /w)
    set_target_properties(${TARGET} PROPERTIES COMPILE_OPTIONS ${target_options})
  endif()
  set_target_properties(${TARGET} PROPERTIES WARNINGS_ENABLED OFF)
endfunction()

function(target_enable_clang_tidy TARGET)
  get_target_property(warnings_enabled ${TARGET} WARNINGS_ENABLED)
  if(warnings_enabled)
    set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY "clang-tidy;--warnings-as-errors=*")
  else()
    set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY clang-tidy)
  endif()
  set_target_properties(${TARGET} PROPERTIES CLANG_TIDY_ENABLED ON)
endfunction()

# Enable warnings for targets, which called target_enable_coding_standards
function(activate_warnings)
  set_property(GLOBAL PROPERTY WARNINGS_ACTIVATED ON)
endfunction()

# Enable clang-tidy for targets, which called target_enable_coding_standards
function(activate_clang_tidy)
  set_property(GLOBAL PROPERTY CLANG_TIDY_ACTIVATED ON)
endfunction()

function(target_enable_coding_standards TARGET)
  get_property(warnings_activated GLOBAL PROPERTY WARNINGS_ACTIVATED)
  if(warnings_activated)
    target_enable_warnings(${TARGET})
  endif()

  get_property(clang_tidy_activated GLOBAL PROPERTY CLANG_TIDY_ACTIVATED)
  if(clang_tidy_activated)
    target_enable_clang_tidy(${TARGET})
  endif()

endfunction()

# Enable coding standards for ALL targets, even if they do not ask for
function(force_coding_standards)
  get_targets(targets LIVE)
  foreach(target ${targets})
    target_enable_coding_standards(target)
  endforeach()
endfunction()
