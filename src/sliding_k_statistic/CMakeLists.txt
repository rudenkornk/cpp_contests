add_executable(sliding_k_statistic_cli)
target_sources(
  sliding_k_statistic_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(sliding_k_statistic_cli PUBLIC cxx_std_23)
set_target_properties(sliding_k_statistic_cli PROPERTIES OUTPUT_NAME sliding_k_statistic)
target_setup_for_install(sliding_k_statistic_cli)
target_allow_instrumentation(sliding_k_statistic_cli)
target_allow_coding_standards(sliding_k_statistic_cli)

add_subdirectory(tests)
