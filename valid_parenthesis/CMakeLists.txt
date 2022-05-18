add_executable(valid_parenthesis_cli)

target_sources(valid_parenthesis_cli PRIVATE solution.cpp)
target_compile_features(valid_parenthesis_cli PRIVATE cxx_std_20)
target_compile_options(
  valid_parenthesis_cli
  PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
          $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

add_subdirectory(lit_tests)
