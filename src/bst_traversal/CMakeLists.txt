add_executable(bst_traversal_cli)
target_sources(
  bst_traversal_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(bst_traversal_cli PUBLIC cxx_std_23)
set_target_properties(bst_traversal_cli PROPERTIES OUTPUT_NAME bst_traversal)
target_setup_for_install(bst_traversal_cli)
target_allow_instrumentation(bst_traversal_cli)
target_allow_coding_standards(bst_traversal_cli)

add_subdirectory(tests)
