add_library(reverse_list INTERFACE)
target_sources(
  reverse_list
  INTERFACE FILE_SET
            reverse_list_headers
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            include/reverse_list/solution.hpp)
target_compile_features(reverse_list INTERFACE cxx_std_20)
target_setup_for_install(reverse_list)

add_subdirectory(unit_tests)
