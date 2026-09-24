add_executable(min_max_arrays_cli)
target_sources(
  min_max_arrays_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(min_max_arrays_cli PUBLIC cxx_std_23)
set_target_properties(min_max_arrays_cli PROPERTIES OUTPUT_NAME min_max_arrays)
target_setup_for_install(min_max_arrays_cli)
target_allow_instrumentation(min_max_arrays_cli)
target_allow_coding_standards(min_max_arrays_cli)

add_subdirectory(tests)
