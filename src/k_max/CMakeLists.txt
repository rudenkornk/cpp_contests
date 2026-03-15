add_library(k_max)

target_sources(
  k_max
  PUBLIC FILE_SET
         CXX_MODULES
         FILES
           modules/solution.cppm
)

target_compile_features(k_max PUBLIC cxx_std_23)
target_setup_for_install(k_max)
target_allow_instrumentation(k_max)
target_allow_coding_standards(k_max)

add_subdirectory(tests)
