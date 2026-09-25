add_executable(minimal_covering_circle_cli)
target_sources(
  minimal_covering_circle_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(minimal_covering_circle_cli PUBLIC cxx_std_23)
set_target_properties(minimal_covering_circle_cli PROPERTIES OUTPUT_NAME minimal_covering_circle)
target_setup_for_install(minimal_covering_circle_cli)
target_allow_instrumentation(minimal_covering_circle_cli)
target_allow_coding_standards(minimal_covering_circle_cli)

add_subdirectory(tests)
