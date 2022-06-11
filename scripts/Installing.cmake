include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(Utils)

define_property(TARGET PROPERTY ADD_TO_INSTALLATION)

function(target_register_for_install TARGET)
  set_property(TARGET ${TARGET} PROPERTY ADD_TO_INSTALLATION ON)
endfunction()

function(generic_install)
  set(config ${CMAKE_PROJECT_NAME}-config.cmake)
  set(config_version ${CMAKE_PROJECT_NAME}-config-version.cmake)
  set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME})

  get_targets(install_targets BOOL_PROPERTIES ADD_TO_INSTALLATION)
  if(NOT install_targets)
    message(WARNING "Nothing to install.")
    return()
  endif()

  configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/scripts/config.cmake.in ${CMAKE_BINARY_DIR}/${config}
    INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_BINDIR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_SYSCONFDIR
              CMAKE_INSTALL_CMAKEDIR)
  write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/${config_version}
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

  foreach(target ${install_targets})
    get_target_property(header_sets ${target} INTERFACE_HEADER_SETS)
    foreach(header_set ${header_sets})
      list(APPEND INSTALL_HEADER_SET_OPTION FILE_SET ${header_set} COMPONENT ${CMAKE_PROJECT_NAME}_dev)
    endforeach()
  endforeach()

  install(
    TARGETS ${install_targets}
    EXPORT ${CMAKE_PROJECT_NAME}_targets
    RUNTIME COMPONENT ${CMAKE_PROJECT_NAME}_runtime
    LIBRARY COMPONENT ${CMAKE_PROJECT_NAME}_runtime NAMELINK_COMPONENT ${CMAKE_PROJECT_NAME}_dev
    ARCHIVE COMPONENT ${CMAKE_PROJECT_NAME}_dev ${INSTALL_HEADER_SET_OPTION}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(
    EXPORT ${CMAKE_PROJECT_NAME}_targets
    COMPONENT ${CMAKE_PROJECT_NAME}_dev
    NAMESPACE ${CMAKE_PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR})
  install(
    FILES ${CMAKE_BINARY_DIR}/${config} ${CMAKE_BINARY_DIR}/${config_version}
    COMPONENT ${CMAKE_PROJECT_NAME}_dev
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR})
endfunction()
