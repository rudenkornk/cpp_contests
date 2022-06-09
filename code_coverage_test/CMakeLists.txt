add_library(code_coverage_test)
target_include_directories(code_coverage_test PUBLIC .)
target_sources(code_coverage_test PRIVATE
  solution.hpp
  solution.cpp
  )
target_compile_features(code_coverage_test PRIVATE cxx_std_20)
target_enable_instrumentation(code_coverage_test)
target_enable_coding_standards(code_coverage_test)

add_executable(code_coverage_test_cli)

target_sources(code_coverage_test_cli PRIVATE cli.cpp)
target_add_headers_as_sources(code_coverage_test_cli code_coverage_test)
target_link_libraries(code_coverage_test_cli PRIVATE code_coverage_test)
target_compile_features(code_coverage_test_cli PRIVATE cxx_std_20)
set_target_properties(code_coverage_test_cli PROPERTIES OUTPUT_NAME
                                                       code_coverage_test)

target_enable_instrumentation(code_coverage_test_cli)
target_enable_coding_standards(code_coverage_test_cli)

add_subdirectory(lit_tests)
