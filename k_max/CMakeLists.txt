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
target_compile_features(k_max PRIVATE cxx_std_20)
target_register_for_install(k_max)

add_subdirectory(unit_tests)
