add_executable(fixed_set_tests)
target_sources(fixed_set_tests PRIVATE test.cpp)

target_link_libraries(fixed_set_tests PRIVATE utils)
target_link_libraries(fixed_set_tests PRIVATE Boost::headers)
add_dependencies(fixed_set_tests fixed_set_cli)

target_allow_instrumentation(fixed_set_tests)
target_allow_coding_standards(fixed_set_tests)

add_tests(fixed_set_tests)
