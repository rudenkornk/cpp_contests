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
target_include_directories(reverse_list INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(reverse_list INTERFACE cxx_std_20)

add_subdirectory(unit_tests)
