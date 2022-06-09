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
endfunction()

function(target_disable_warnings TARGET)
  if(NOT ${ARGC} EQUAL 1)
    message(FATAL_ERROR "Provide exactly one target.")
  endif()
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    get_target_property(target_options ${TARGET} COMPILE_OPTIONS)
    list(REMOVE_ITEM target_options -Wall -Wextra -pedantic -Werror)
    list(APPEND target_options -w)
    set_property(TARGET ${TARGET} PROPERTY COMPILE_OPTIONS ${target_options})
  elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    get_target_property(target_options ${TARGET} COMPILE_OPTIONS)
    list(REMOVE_ITEM target_options /W4 /WX)
    list(APPEND target_options /w)
    set_property(TARGET ${TARGET} PROPERTY COMPILE_OPTIONS ${target_options})
  endif()
endfunction()

function(enable_warnings)
  set(warnings_enabled
      ON
      PARENT_SCOPE)
endfunction()

function(target_enable_coding_standards TARGET)
  if(warnings_enabled)
    target_enable_warnings(${TARGET})
  endif()
endfunction()