add_executable(tree_isomorphism_cli)
target_sources(
  tree_isomorphism_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(tree_isomorphism_cli PUBLIC cxx_std_23)
set_target_properties(tree_isomorphism_cli PROPERTIES OUTPUT_NAME tree_isomorphism)
target_setup_for_install(tree_isomorphism_cli)
target_allow_instrumentation(tree_isomorphism_cli)
target_allow_coding_standards(tree_isomorphism_cli)

add_subdirectory(tests)
