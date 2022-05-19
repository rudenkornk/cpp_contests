set(test_target k_max)
add_executable(${test_target}_unit_tests)

find_package(Boost REQUIRED)

target_link_libraries(${test_target}_unit_tests PRIVATE ${test_target})

target_include_directories(${test_target}_unit_tests PRIVATE ${BOOST_INCLUDE_DIRS})

file(GLOB unit_tests_sources CONFIGURE_DEPENDS *.[hcm]pp)
target_sources(${test_target}_unit_tests PRIVATE ${unit_tests_sources})

target_compile_options(
  ${test_target}_unit_tests
  PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

add_test(NAME ${test_target}_unit_tests COMMAND ${test_target}_unit_tests)
add_test(NAME ${test_target}_unit_tests_valgrind COMMAND valgrind $<TARGET_FILE:${test_target}_unit_tests>)
