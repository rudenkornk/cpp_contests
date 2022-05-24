add_executable(k_max_unit_tests)

find_package(Boost REQUIRED)

target_link_libraries(k_max_unit_tests PRIVATE k_max)

target_include_directories(k_max_unit_tests SYSTEM
                           PRIVATE ${BOOST_INCLUDE_DIRS})

file(GLOB unit_tests_sources CONFIGURE_DEPENDS *.[hcm]pp)
target_sources(k_max_unit_tests PRIVATE ${unit_tests_sources})

target_enable_instrumentation(k_max_unit_tests)

add_unit_tests(NAME k_max_unit_tests COMMAND k_max_unit_tests)
