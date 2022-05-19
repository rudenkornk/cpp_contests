add_library(k_max INTERFACE)
target_include_directories(k_max INTERFACE .)
target_compile_features(k_max INTERFACE cxx_std_20)

add_subdirectory(unit_tests)
