add_library(lru_cache INTERFACE)
target_sources(
  lru_cache
  INTERFACE FILE_SET
            lru_cache_headers
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            include/lru_cache/solution.hpp)
target_compile_features(lru_cache INTERFACE cxx_std_23)
target_setup_for_install(lru_cache)

add_executable(lru_cache_cli)
target_sources(lru_cache_cli PRIVATE cli.cpp)
target_add_headers_as_sources(lru_cache_cli lru_cache)
target_link_libraries(lru_cache_cli PRIVATE lru_cache)
set_target_properties(lru_cache_cli PROPERTIES OUTPUT_NAME lru_cache)
target_setup_for_install(lru_cache_cli)
target_enable_instrumentation(lru_cache_cli)
target_enable_coding_standards(lru_cache_cli)

add_subdirectory(lit_tests)
