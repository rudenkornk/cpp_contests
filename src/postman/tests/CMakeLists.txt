add_executable(postman_tests)
target_sources(postman_tests PRIVATE test.cpp)

target_link_libraries(postman_tests PRIVATE utils)
target_link_libraries(postman_tests PRIVATE Boost::headers)
add_dependencies(postman_tests postman_cli)

target_allow_instrumentation(postman_tests)
target_allow_coding_standards(postman_tests)

add_tests(postman_tests)
