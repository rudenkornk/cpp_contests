add_executable(valid_parenthesis_cli)

target_sources(valid_parenthesis_cli PRIVATE solution.cpp)
target_compile_features(valid_parenthesis_cli PRIVATE cxx_std_20)
set_target_properties(valid_parenthesis_cli PROPERTIES OUTPUT_NAME
                                                       valid_parenthesis)

target_enable_instrumentation(valid_parenthesis_cli)
target_enable_coding_standards(valid_parenthesis_cli)


add_subdirectory(lit_tests)
