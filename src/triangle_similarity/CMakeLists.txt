add_executable(triangle_similarity_cli)
target_sources(
  triangle_similarity_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(triangle_similarity_cli PUBLIC cxx_std_23)
set_target_properties(triangle_similarity_cli PROPERTIES OUTPUT_NAME triangle_similarity)
target_setup_for_install(triangle_similarity_cli)
target_allow_instrumentation(triangle_similarity_cli)
target_allow_coding_standards(triangle_similarity_cli)

add_subdirectory(tests)
