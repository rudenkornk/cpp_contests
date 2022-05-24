add_executable(missing_numbers_unit_tests)

find_package(Boost REQUIRED)

target_link_libraries(missing_numbers_unit_tests PRIVATE missing_numbers)

target_include_directories(missing_numbers_unit_tests SYSTEM
                           PRIVATE ${BOOST_INCLUDE_DIRS})

file(GLOB unit_tests_sources CONFIGURE_DEPENDS *.[hcm]pp)
target_sources(missing_numbers_unit_tests PRIVATE ${unit_tests_sources})

target_enable_instrumentation(missing_numbers_unit_tests)

add_unit_tests(NAME missing_numbers_unit_tests COMMAND missing_numbers_unit_tests)
