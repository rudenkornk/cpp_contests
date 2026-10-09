add_executable(merge_sorted_arrays_cli)
target_sources(
  merge_sorted_arrays_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(merge_sorted_arrays_cli PUBLIC cxx_std_23)
set_target_properties(merge_sorted_arrays_cli PROPERTIES OUTPUT_NAME merge_sorted_arrays)
target_setup_for_install(merge_sorted_arrays_cli)
target_allow_instrumentation(merge_sorted_arrays_cli)
target_allow_coding_standards(merge_sorted_arrays_cli)

add_subdirectory(tests)
