add_library(k_max INTERFACE)
target_sources(
  k_max
  INTERFACE FILE_SET
            k_max_headers
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            include/k_max/solution.hpp)
target_include_directories(k_max INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_features(k_max INTERFACE cxx_std_20)

add_subdirectory(unit_tests)
