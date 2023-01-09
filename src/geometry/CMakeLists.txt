add_library(geometry INTERFACE)
target_sources(
  geometry
  INTERFACE FILE_SET
            geometry_headers
            TYPE
            HEADERS
            BASE_DIRS
            include
            FILES
            include/geometry/matrix.hpp
            include/geometry/primitives.hpp
            include/geometry/triangle.hpp
)
target_compile_features(geometry INTERFACE cxx_std_23)
target_setup_for_install(geometry)

target_link_libraries(geometry INTERFACE utils)
add_subdirectory(unit_tests)
