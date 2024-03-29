include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(Utils)

define_property(TARGET PROPERTY ADD_TO_INSTALLATION)

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN)
endif()

function(_target_setup_symbols_visibility TARGET)
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
  if(type MATCHES "LIBRARY")
    add_library(${CMAKE_PROJECT_NAME}::${TARGET} ALIAS ${TARGET})
    if(NOT type STREQUAL "INTERFACE_LIBRARY")
      _target_setup_symbols_visibility(${TARGET})
    endif()
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
    ${PROJECT_SOURCE_DIR}/cmake/package_config/config.cmake.in ${PROJECT_BINARY_DIR}/${config}
    INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_BINDIR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_SYSCONFDIR
              CMAKE_INSTALL_CMAKEDIR)
  write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${config_version}
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
    FILES ${PROJECT_BINARY_DIR}/${config} ${PROJECT_BINARY_DIR}/${config_version}
    COMPONENT ${CMAKE_PROJECT_NAME}_dev
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR})
endfunction()

function(_set_test_install_find_packages)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs FIND_PACKAGES)
  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()

  list(SORT ARG_FIND_PACKAGES)
  foreach(package ${ARG_FIND_PACKAGES})
    string(APPEND find_packages "find_package(")
    string(APPEND find_packages ${package})
    string(APPEND find_packages " REQUIRED)\n")
  endforeach()

  set(FIND_PACKAGES
      ${find_packages}
      PARENT_SCOPE)
endfunction()

function(_set_test_install_vcpkg)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs VCPKG_DEPS)
  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()

  list(SORT ARG_VCPKG_DEPS)
  set(vcpkg_next "") # Workaround for weird json syntax
  foreach(dependency ${ARG_VCPKG_DEPS})
    string(APPEND vcpkg_deps ${vcpkg_comma})
    set(vcpkg_next ",\n")
    string(APPEND vcpkg_deps "\"")
    string(APPEND vcpkg_deps ${dependency})
    string(APPEND vcpkg_deps "\"")
  endforeach()

  file(READ ${PROJECT_SOURCE_DIR}/vcpkg.json vcpkg_json)
  string(JSON baseline GET ${vcpkg_json} "builtin-baseline")

  set(VCPKG_DEPS
      ${vcpkg_deps}
      PARENT_SCOPE)

  set(VCPKG_BASELINE
      ${baseline}
      PARENT_SCOPE)
endfunction()

function(_set_test_install_binaries)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs)
  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()

  get_targets(install_targets BOOL_PROPERTIES ADD_TO_INSTALLATION)
  if(NOT install_targets)
    message(WARNING "No install targets to test.")
    return()
  endif()
  foreach(target ${install_targets})
    get_target_property(type ${target} TYPE)
    if(type STREQUAL "EXECUTABLE")
      list(APPEND installed_executables ${CMAKE_PROJECT_NAME}::${target})
    else()
      get_target_property(header_sets ${target} INTERFACE_HEADER_SETS)
      foreach(header_set ${header_sets})
        get_target_property(header_files ${target} HEADER_SET_${header_set})
        get_target_property(header_dirs ${target} HEADER_DIRS_${header_set})
        foreach(header_file ${header_files})
          get_relative_path(header PATH ${header_file} BASE_DIRS ${header_dirs})
          string(APPEND cpp_includes "#include <${header}>\n")
        endforeach()
      endforeach()
      list(APPEND installed_libraries ${CMAKE_PROJECT_NAME}::${target})
    endif()
  endforeach()

  set(INSTALLED_EXECUTABLES
      ${installed_executables}
      PARENT_SCOPE)
  set(INSTALLED_LIBRARIES
      ${installed_libraries}
      PARENT_SCOPE)
  set(CPP_INCLUDES
      ${cpp_includes}
      PARENT_SCOPE)
endfunction()

# Create a simple project, which checks that all targets and headers were exported
function(configure_test_install_project)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs FIND_PACKAGES VCPKG_DEPS)
  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()

  _set_test_install_find_packages(FIND_PACKAGES ${ARG_FIND_PACKAGES})
  _set_test_install_vcpkg(VCPKG_DEPS ${ARG_VCPKG_DEPS})
  _set_test_install_binaries()

  set(in_path ${PROJECT_SOURCE_DIR}/cmake/test_install)
  configure_file(${in_path}/vcpkg.json.in ${PROJECT_BINARY_DIR}/test_install/vcpkg.json @ONLY)
  configure_file(${in_path}/CMakeLists.txt.in ${PROJECT_BINARY_DIR}/test_install/CMakeLists.txt @ONLY)
  configure_file(${in_path}/CMakePresets.json.in ${PROJECT_BINARY_DIR}/test_install/CMakePresets.json @ONLY)
  configure_file(${in_path}/main.cpp.in ${PROJECT_BINARY_DIR}/test_install/main.cpp @ONLY)
endfunction()

function(configure_cpack_options)
  set(options)
  set(oneValueArgs EMAIL)
  set(multiValueArgs)
  cmake_parse_arguments(PARSE_ARGV 0 ARG "${options}" "${oneValueArgs}" "${multiValueArgs}")

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Too many arguments.")
  endif()
  if(NOT DEFINED ARG_EMAIL)
    message(FATAL_ERROR "EMAIL is required argument.")
  endif()

  set(EMAIL ${ARG_EMAIL})
  configure_file("${PROJECT_SOURCE_DIR}/cmake/package_config/CPackOptions.cmake.in"
                 "${PROJECT_BINARY_DIR}/CPackOptions.cmake" @ONLY)
  set(CPACK_PROJECT_CONFIG_FILE
      "${PROJECT_BINARY_DIR}/CPackOptions.cmake"
      PARENT_SCOPE)
endfunction()
