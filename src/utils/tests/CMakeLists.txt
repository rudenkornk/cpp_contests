add_executable(utils_tests)

target_link_libraries(utils_tests PRIVATE utils)
target_link_libraries(utils_tests PRIVATE Boost::headers)

target_sources(utils_tests PRIVATE test_1.cpp test_2.cpp)

target_allow_instrumentation(utils_tests)
target_allow_coding_standards(utils_tests)

add_tests(utils_tests)
