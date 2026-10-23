add_executable(fixed_set_cli)
target_sources(
  fixed_set_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(fixed_set_cli PUBLIC cxx_std_23)
set_target_properties(fixed_set_cli PROPERTIES OUTPUT_NAME fixed_set)
target_setup_for_install(fixed_set_cli)
target_allow_instrumentation(fixed_set_cli)
target_allow_coding_standards(fixed_set_cli)

add_subdirectory(tests)
