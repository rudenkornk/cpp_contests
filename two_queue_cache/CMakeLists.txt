add_library(two_queue_cache INTERFACE)
target_sources(
  two_queue_cache
  INTERFACE FILE_SET
            two_queue_cache_headers
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            include/two_queue_cache/solution.hpp)
target_compile_features(two_queue_cache INTERFACE cxx_std_20)
target_setup_for_install(two_queue_cache)

add_executable(two_queue_cache_cli)
target_sources(two_queue_cache_cli PRIVATE cli.cpp)
target_add_headers_as_sources(two_queue_cache_cli two_queue_cache)
target_link_libraries(two_queue_cache_cli PRIVATE two_queue_cache)
set_target_properties(two_queue_cache_cli PROPERTIES OUTPUT_NAME two_queue_cache)
target_setup_for_install(two_queue_cache_cli)
target_enable_instrumentation(two_queue_cache_cli)
target_enable_coding_standards(two_queue_cache_cli)

add_subdirectory(lit_tests)
