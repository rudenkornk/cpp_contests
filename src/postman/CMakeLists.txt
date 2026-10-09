add_executable(postman_cli)
target_sources(
  postman_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(postman_cli PUBLIC cxx_std_23)
set_target_properties(postman_cli PROPERTIES OUTPUT_NAME postman)
target_setup_for_install(postman_cli)
target_allow_instrumentation(postman_cli)
target_allow_coding_standards(postman_cli)

add_subdirectory(tests)
