include(CheckIPOSupported)

define_property(GLOBAL PROPERTY LTO_ENABLED)

function(target_enable_lto TARGET)
  set_property(TARGET ${TARGET} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endfunction()

function(enable_lto)
  check_ipo_supported(RESULT supported OUTPUT error)
  if(NOT supported)
    message(FATAL_ERROR "LTO not supported: ${error}")
  endif()
  set_property(GLOBAL PROPERTY LTO ON)
endfunction()

function(target_enable_optimizaions TARGET)
  get_property(lto_enabled GLOBAL PROPERTY LTO)
  if(lto_enabled)
    target_enable_lto(${TARGET})
  endif()

endfunction()
