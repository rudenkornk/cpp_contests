add_executable(memory_manager_cli)
target_sources(
  memory_manager_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(memory_manager_cli PUBLIC cxx_std_23)
set_target_properties(memory_manager_cli PROPERTIES OUTPUT_NAME memory_manager)
target_setup_for_install(memory_manager_cli)
target_allow_instrumentation(memory_manager_cli)
target_allow_coding_standards(memory_manager_cli)

add_subdirectory(tests)
