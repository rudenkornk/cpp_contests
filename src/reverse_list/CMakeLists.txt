add_library(reverse_list)

target_sources(
  reverse_list
  PUBLIC FILE_SET
         CXX_MODULES
         FILES
           modules/solution.cppm
)

target_compile_features(reverse_list PUBLIC cxx_std_23)
target_setup_for_install(reverse_list)
target_allow_instrumentation(reverse_list)
target_allow_coding_standards(reverse_list)

add_subdirectory(tests)
