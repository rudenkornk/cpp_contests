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
            include/utils/math.hpp
            include/utils/type_traits.hpp
            include/utils/utils.hpp
            include/utils/warnings.hpp)
target_compile_features(utils INTERFACE cxx_std_23)
target_link_libraries(utils INTERFACE fmt::fmt)
target_setup_for_install(utils)

add_subdirectory(unit_tests)
