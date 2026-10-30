add_executable(count_search_trees_cli)
target_sources(
  count_search_trees_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            solution.cppm
)
target_compile_features(count_search_trees_cli PUBLIC cxx_std_23)
set_target_properties(count_search_trees_cli PROPERTIES OUTPUT_NAME count_search_trees)
target_setup_for_install(count_search_trees_cli)
target_allow_instrumentation(count_search_trees_cli)
target_allow_coding_standards(count_search_trees_cli)

add_subdirectory(tests)
