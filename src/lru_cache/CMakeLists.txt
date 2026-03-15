add_library(lru_cache)

target_sources(
  lru_cache
  PUBLIC FILE_SET
         CXX_MODULES
         FILES
           modules/solution.cppm
)

target_compile_features(lru_cache PUBLIC cxx_std_23)
target_setup_for_install(lru_cache)
target_allow_instrumentation(lru_cache)
target_allow_coding_standards(lru_cache)

add_executable(lru_cache_cli)
target_sources(lru_cache_cli PRIVATE cli.cpp)
target_link_libraries(lru_cache_cli PRIVATE lru_cache)
set_target_properties(lru_cache_cli PROPERTIES OUTPUT_NAME lru_cache)
target_setup_for_install(lru_cache_cli)
target_allow_instrumentation(lru_cache_cli)
target_allow_coding_standards(lru_cache_cli)

add_subdirectory(tests)
