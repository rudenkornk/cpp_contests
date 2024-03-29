add_library(missing_numbers INTERFACE)
target_sources(
  missing_numbers
  INTERFACE FILE_SET
            missing_numbers_headers
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            include/missing_numbers/solution.hpp)
target_compile_features(missing_numbers INTERFACE cxx_std_23)
target_setup_for_install(missing_numbers)

add_executable(missing_numbers_cli)
target_sources(missing_numbers_cli PRIVATE cli.cpp)
target_add_headers_as_sources(missing_numbers_cli missing_numbers)
target_link_libraries(missing_numbers_cli PRIVATE missing_numbers)
set_target_properties(missing_numbers_cli PROPERTIES OUTPUT_NAME missing_numbers)
target_setup_for_install(missing_numbers_cli)
target_enable_instrumentation(missing_numbers_cli)
target_enable_coding_standards(missing_numbers_cli)

add_subdirectory(unit_tests)
add_subdirectory(lit_tests)
