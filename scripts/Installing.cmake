include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(Utils)

define_property(TARGET PROPERTY ADD_TO_INSTALLATION)

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

function(target_setup_symbols_visibility TARGET)
  set_target_properties(${TARGET} PROPERTIES VISIBILITY_INLINES_HIDDEN ON)
  set_target_properties(${TARGET} PROPERTIES CXX_VISIBILITY_PRESET hidden)
  generate_export_header(${TARGET})

  set(base_dir ${CMAKE_CURRENT_BINARY_DIR}/..)
  cmake_path(NORMAL_PATH base_dir)
  target_sources(
    ${TARGET}
    PUBLIC FILE_SET
           ${TARGET}_export_header
           TYPE
           HEADERS
           BASE_DIRS
           ${base_dir}
           FILES
           ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_export.h)
endfunction()

function(target_setup_for_install TARGET)
  string(REGEX MATCH "^[0-9]+" soversion ${CMAKE_PROJECT_VERSION})
  set_target_properties(${TARGET} PROPERTIES SOVERSION ${soversion} VERSION ${CMAKE_PROJECT_VERSION})
  get_target_property(type ${TARGET} TYPE)
  if((type MATCHES "LIBRARY") AND NOT (type STREQUAL "INTERFACE_LIBRARY"))
    target_setup_symbols_visibility(${TARGET})
  endif()
  set_target_properties(${TARGET} PROPERTIES ADD_TO_INSTALLATION ON)
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

# Create a simple project, which checks that all targets and headers were exported
function(configure_test_install_project)
  get_targets(install_targets BOOL_PROPERTIES ADD_TO_INSTALLATION)
  if(NOT install_targets)
    message(WARNING "No install targets to test.")
    return()
  endif()
  foreach(target ${install_targets})
    get_target_property(type ${target} TYPE)
    if(type STREQUAL "EXECUTABLE")
      list(APPEND INSTALLED_EXECUTABLES ${CMAKE_PROJECT_NAME}::${target})
    else()
      get_target_property(header_sets ${target} INTERFACE_HEADER_SETS)
      foreach(header_set ${header_sets})
        get_target_property(header_files ${target} HEADER_SET_${header_set})
        get_target_property(header_dirs ${target} HEADER_DIRS_${header_set})
        foreach(header_file ${header_files})
          get_relative_path(header PATH ${header_file} BASE_DIRS ${header_dirs})
          string(APPEND CPP_INCLUDES "#include <${header}>\n")
        endforeach()
      endforeach()
      list(APPEND INSTALLED_LIBRARIES ${CMAKE_PROJECT_NAME}::${target})
    endif()
  endforeach()

  configure_file(${PROJECT_SOURCE_DIR}/scripts/test_install/CMakeLists.txt.in
                 ${CMAKE_BINARY_DIR}/test_install/CMakeLists.txt @ONLY)
  configure_file(${PROJECT_SOURCE_DIR}/scripts/test_install/main.cpp.in ${CMAKE_BINARY_DIR}/test_install/main.cpp @ONLY)
endfunction()
