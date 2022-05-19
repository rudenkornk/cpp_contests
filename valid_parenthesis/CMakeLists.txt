add_executable(valid_parenthesis_cli)

target_sources(valid_parenthesis_cli PRIVATE solution.cpp)
target_compile_features(valid_parenthesis_cli PRIVATE cxx_std_20)
target_compile_options(
  valid_parenthesis_cli
  PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)
set_target_properties(valid_parenthesis_cli PROPERTIES OUTPUT_NAME valid_parenthesis)

add_subdirectory(lit_tests)
