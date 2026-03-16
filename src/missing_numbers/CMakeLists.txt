add_library(missing_numbers)

target_sources(
  missing_numbers
  PUBLIC FILE_SET
         CXX_MODULES
         FILES
           modules/solution.cppm
)

target_compile_features(missing_numbers PUBLIC cxx_std_23)
target_setup_for_install(missing_numbers)
target_allow_instrumentation(missing_numbers)
target_allow_coding_standards(missing_numbers)

add_executable(missing_numbers_cli)
target_sources(
  missing_numbers_cli
  PUBLIC FILE_SET
          CXX_MODULES
          FILES
            cli.cppm
)
target_compile_features(missing_numbers_cli PUBLIC cxx_std_23)
target_link_libraries(missing_numbers_cli PRIVATE missing_numbers)
set_target_properties(missing_numbers_cli PROPERTIES OUTPUT_NAME missing_numbers)
target_setup_for_install(missing_numbers_cli)
target_allow_instrumentation(missing_numbers_cli)
target_allow_coding_standards(missing_numbers_cli)

add_subdirectory(tests)
