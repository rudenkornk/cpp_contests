add_library(k_max)
target_sources(
  k_max
  PUBLIC FILE_SET
         k_max_headers
         TYPE
         HEADERS
         BASE_DIRS
         include
         FILES
         include/k_max/solution.hpp)
target_sources(k_max PRIVATE solution.cpp)
target_compile_features(k_max PRIVATE cxx_std_23)
target_setup_for_install(k_max)
target_allow_instrumentation(k_max)
target_allow_coding_standards(k_max)

add_subdirectory(tests)
