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
target_compile_features(perfect_cache PRIVATE cxx_std_23)
target_setup_for_install(perfect_cache)
target_enable_instrumentation(perfect_cache)
target_enable_coding_standards(perfect_cache)

add_executable(perfect_cache_cli)
target_sources(perfect_cache_cli PRIVATE cli.cpp)
target_link_libraries(perfect_cache_cli PRIVATE perfect_cache)
set_target_properties(perfect_cache_cli PROPERTIES OUTPUT_NAME perfect_cache)
target_setup_for_install(perfect_cache_cli)
target_enable_instrumentation(perfect_cache_cli)
target_enable_coding_standards(perfect_cache_cli)

add_subdirectory(unit_tests)
