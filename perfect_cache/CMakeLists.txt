add_library(perfect_cache)
target_sources(
  perfect_cache
  PUBLIC FILE_SET
         perfect_cache_headers
         TYPE
         HEADERS
         BASE_DIRS
         include
         FILES
         include/perfect_cache/solution.hpp)
target_sources(perfect_cache PRIVATE solution.cpp)
target_compile_features(perfect_cache PRIVATE cxx_std_20)
target_setup_for_install(perfect_cache)
target_enable_instrumentation(perfect_cache)
target_enable_coding_standards(perfect_cache)

add_subdirectory(unit_tests)
