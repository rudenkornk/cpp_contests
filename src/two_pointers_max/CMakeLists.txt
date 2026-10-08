add_executable(two_pointers_max_cli)
target_sources(
  two_pointers_max_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(two_pointers_max_cli PUBLIC cxx_std_23)
set_target_properties(two_pointers_max_cli PROPERTIES OUTPUT_NAME two_pointers_max)
target_setup_for_install(two_pointers_max_cli)
target_allow_instrumentation(two_pointers_max_cli)
target_allow_coding_standards(two_pointers_max_cli)

add_subdirectory(tests)
