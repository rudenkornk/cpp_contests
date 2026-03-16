add_executable(valid_parenthesis_cli)
target_sources(
  valid_parenthesis_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(valid_parenthesis_cli PUBLIC cxx_std_23)
set_target_properties(valid_parenthesis_cli PROPERTIES OUTPUT_NAME valid_parenthesis)
target_setup_for_install(valid_parenthesis_cli)
target_allow_instrumentation(valid_parenthesis_cli)
target_allow_coding_standards(valid_parenthesis_cli)

add_subdirectory(tests)
