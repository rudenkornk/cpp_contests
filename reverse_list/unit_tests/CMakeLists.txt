add_executable(reverse_list_unit_tests)

find_package(Boost REQUIRED)

target_link_libraries(reverse_list_unit_tests PRIVATE reverse_list)

target_include_directories(reverse_list_unit_tests SYSTEM
                           PRIVATE ${Boost_INCLUDE_DIRS})

file(GLOB unit_tests_sources CONFIGURE_DEPENDS *.[hcm]pp)
target_sources(reverse_list_unit_tests PRIVATE ${unit_tests_sources})
target_enable_instrumentation(reverse_list_unit_tests)

add_unit_tests(NAME reverse_list_unit_tests TARGET reverse_list_unit_tests)
