include(CheckIPOSupported)

define_property(GLOBAL PROPERTY LTO_ACTIVATED)

function(target_enable_lto TARGET)
  set(options)
  set(oneValueArgs CONFIG)
  set(multiValueArgs)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()
  if(NOT DEFINED ARG_CONFIG)
    set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION_${ARG_CONFIG} TRUE)
  endif()

endfunction()

function(activate_lto)
  check_ipo_supported(RESULT supported OUTPUT error)
  if(NOT supported)
    message(FATAL_ERROR "LTO not supported: ${error}")
  endif()
  set_property(GLOBAL PROPERTY LTO_ACTIVATED ON)
endfunction()

function(target_enable_optimizaions TARGET)
  get_property(lto_activated GLOBAL PROPERTY LTO_ACTIVATED)
  if(lto_activated)
    target_enable_lto(${TARGET})
  endif()

endfunction()
