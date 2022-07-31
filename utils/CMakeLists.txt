add_library(utils INTERFACE)
target_sources(
  utils
  INTERFACE FILE_SET
            utils_headers
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            include/utils/utils.hpp)
target_compile_features(utils INTERFACE cxx_std_20)
target_link_libraries(utils INTERFACE fmt::fmt)
target_setup_for_install(utils)

add_subdirectory(unit_tests)
