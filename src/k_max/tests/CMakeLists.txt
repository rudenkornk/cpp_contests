add_executable(k_max_tests)

target_link_libraries(k_max_tests PRIVATE k_max)
target_link_libraries(k_max_tests PRIVATE Boost::headers)

target_sources(k_max_tests PRIVATE test_1.cpp)
target_compile_features(k_max_tests PRIVATE cxx_std_23)

target_allow_instrumentation(k_max_tests)
target_allow_coding_standards(k_max_tests)

add_tests(k_max_tests)
