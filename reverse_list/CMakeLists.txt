add_library(reverse_list INTERFACE)
target_include_directories(reverse_list INTERFACE .)
target_compile_features(reverse_list INTERFACE cxx_std_20)

add_subdirectory(unit_tests)
