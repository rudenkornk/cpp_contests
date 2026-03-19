add_executable(lru_cache_tests)

target_sources(lru_cache_tests PRIVATE test.cpp)

target_link_libraries(lru_cache_tests PRIVATE Boost::headers utils)
target_link_libraries(lru_cache_tests PRIVATE lru_cache)
add_dependencies(lru_cache_tests lru_cache_cli)

target_allow_instrumentation(lru_cache_tests)
target_allow_coding_standards(lru_cache_tests)

add_tests(lru_cache_tests)
