add_executable(artifacts_cli)
target_sources(
  artifacts_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(artifacts_cli PUBLIC cxx_std_23)
set_target_properties(artifacts_cli PROPERTIES OUTPUT_NAME artifacts)
target_setup_for_install(artifacts_cli)
target_allow_instrumentation(artifacts_cli)
target_allow_coding_standards(artifacts_cli)

add_subdirectory(tests)
