add_executable(reverse_list_unit_tests)

find_package(Boost REQUIRED)

target_link_libraries(reverse_list_unit_tests PRIVATE reverse_list)

target_include_directories(reverse_list_unit_tests PRIVATE ${BOOST_INCLUDE_DIRS})

file(GLOB unit_tests_sources CONFIGURE_DEPENDS *.[hcm]pp)
target_sources(reverse_list_unit_tests PRIVATE ${unit_tests_sources})

target_enable_warnings(reverse_list_unit_tests)

add_test(NAME reverse_list_unit_tests COMMAND reverse_list_unit_tests)
add_test(NAME reverse_list_unit_tests_valgrind COMMAND valgrind $<TARGET_FILE:reverse_list_unit_tests>)
