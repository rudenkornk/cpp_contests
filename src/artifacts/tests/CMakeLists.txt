add_executable(artifacts_tests)
target_sources(artifacts_tests PRIVATE test.cpp)

target_link_libraries(artifacts_tests PRIVATE utils)
target_link_libraries(artifacts_tests PRIVATE Boost::headers)
add_dependencies(artifacts_tests artifacts_cli)

target_allow_instrumentation(artifacts_tests)
target_allow_coding_standards(artifacts_tests)

add_tests(artifacts_tests)
