add_library(utils)

target_sources(
  utils
  PUBLIC FILE_SET
         CXX_MODULES
         FILES
           modules/math.cppm
           modules/shell.cppm
           modules/test_utils.cppm
           modules/type_traits.cppm
           modules/generic.cppm
           modules/warnings.cppm
)

target_compile_features(utils PUBLIC cxx_std_23)
target_link_libraries(utils PUBLIC Boost::headers)
target_setup_for_install(utils)
target_allow_instrumentation(utils)
target_allow_coding_standards(utils)

add_subdirectory(tests)
