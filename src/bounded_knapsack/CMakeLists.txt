add_executable(bounded_knapsack_cli)
target_sources(
  bounded_knapsack_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(bounded_knapsack_cli PUBLIC cxx_std_23)
set_target_properties(bounded_knapsack_cli PROPERTIES OUTPUT_NAME bounded_knapsack)
target_setup_for_install(bounded_knapsack_cli)
target_allow_instrumentation(bounded_knapsack_cli)
target_allow_coding_standards(bounded_knapsack_cli)

add_subdirectory(tests)
