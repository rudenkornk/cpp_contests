add_library(missing_numbers INTERFACE)
target_include_directories(missing_numbers INTERFACE .)
target_compile_features(missing_numbers INTERFACE cxx_std_20)

add_executable(missing_numbers_cli)
target_sources(missing_numbers_cli PRIVATE cli.cpp)
target_link_libraries(missing_numbers_cli missing_numbers)
target_compile_options(
  missing_numbers_cli
  PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)
set_target_properties(missing_numbers_cli PROPERTIES OUTPUT_NAME missing_numbers)

add_subdirectory(unit_tests)
add_subdirectory(lit_tests)
