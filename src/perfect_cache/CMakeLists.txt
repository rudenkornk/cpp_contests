add_library(perfect_cache)

target_sources(
  perfect_cache
  PUBLIC FILE_SET
         CXX_MODULES
         FILES
           modules/solution.cppm
)

target_compile_features(perfect_cache PUBLIC cxx_std_23)
target_setup_for_install(perfect_cache)
target_allow_instrumentation(perfect_cache)
target_allow_coding_standards(perfect_cache)

add_executable(perfect_cache_cli)
target_sources(
  perfect_cache_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            cli.cppm
)
target_compile_features(perfect_cache_cli PUBLIC cxx_std_23)
target_link_libraries(perfect_cache_cli PRIVATE perfect_cache)
set_target_properties(perfect_cache_cli PROPERTIES OUTPUT_NAME perfect_cache)
target_setup_for_install(perfect_cache_cli)
target_allow_instrumentation(perfect_cache_cli)
target_allow_coding_standards(perfect_cache_cli)

add_subdirectory(tests)
