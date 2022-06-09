add_executable(k_max_unit_tests)

find_package(Boost REQUIRED COMPONENTS headers)

target_link_libraries(k_max_unit_tests PRIVATE k_max)
target_link_libraries(k_max_unit_tests PRIVATE Boost::headers)


file(GLOB unit_tests_sources CONFIGURE_DEPENDS *.[hcm]pp)
target_sources(k_max_unit_tests PRIVATE ${unit_tests_sources})
target_add_headers_as_sources(k_max_unit_tests k_max)

target_enable_instrumentation(k_max_unit_tests)
target_enable_coding_standards(k_max_unit_tests)

add_unit_tests(NAME k_max_unit_tests TARGET k_max_unit_tests)
