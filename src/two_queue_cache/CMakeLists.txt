add_library(two_queue_cache)

target_sources(
  two_queue_cache
  PUBLIC FILE_SET
         CXX_MODULES
         FILES
           modules/solution.cppm
)

target_compile_features(two_queue_cache PUBLIC cxx_std_23)
target_setup_for_install(two_queue_cache)
target_allow_instrumentation(two_queue_cache)
target_allow_coding_standards(two_queue_cache)

add_executable(two_queue_cache_cli)
target_sources(
  two_queue_cache_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            cli.cppm
)
target_compile_features(two_queue_cache_cli PUBLIC cxx_std_23)
target_link_libraries(two_queue_cache_cli PRIVATE two_queue_cache)
set_target_properties(two_queue_cache_cli PROPERTIES OUTPUT_NAME two_queue_cache)
target_setup_for_install(two_queue_cache_cli)
target_allow_instrumentation(two_queue_cache_cli)
target_allow_coding_standards(two_queue_cache_cli)

add_subdirectory(tests)
