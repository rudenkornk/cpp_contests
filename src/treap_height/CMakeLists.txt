add_executable(treap_height_cli)
target_sources(
  treap_height_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(treap_height_cli PUBLIC cxx_std_23)
set_target_properties(treap_height_cli PROPERTIES OUTPUT_NAME treap_height)
target_setup_for_install(treap_height_cli)
target_allow_instrumentation(treap_height_cli)
target_allow_coding_standards(treap_height_cli)

add_subdirectory(tests)
