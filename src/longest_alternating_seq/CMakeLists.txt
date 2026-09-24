add_executable(longest_alternating_seq_cli)
target_sources(
  longest_alternating_seq_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(longest_alternating_seq_cli PUBLIC cxx_std_23)
set_target_properties(longest_alternating_seq_cli PROPERTIES OUTPUT_NAME longest_alternating_seq)
target_setup_for_install(longest_alternating_seq_cli)
target_allow_instrumentation(longest_alternating_seq_cli)
target_allow_coding_standards(longest_alternating_seq_cli)

add_subdirectory(tests)
