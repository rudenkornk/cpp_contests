add_executable(external_rectangles_cli)
target_sources(
  external_rectangles_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(external_rectangles_cli PUBLIC cxx_std_23)
set_target_properties(external_rectangles_cli PROPERTIES OUTPUT_NAME external_rectangles)
target_setup_for_install(external_rectangles_cli)
target_allow_instrumentation(external_rectangles_cli)
target_allow_coding_standards(external_rectangles_cli)

add_subdirectory(tests)
