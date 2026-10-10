add_executable(ali_baba_tests)
target_sources(ali_baba_tests PRIVATE test.cpp)

target_link_libraries(ali_baba_tests PRIVATE utils)
target_link_libraries(ali_baba_tests PRIVATE Boost::headers)
add_dependencies(ali_baba_tests ali_baba_cli)

target_allow_instrumentation(ali_baba_tests)
target_allow_coding_standards(ali_baba_tests)

add_tests(ali_baba_tests)
