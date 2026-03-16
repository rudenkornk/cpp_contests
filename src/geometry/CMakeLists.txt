add_library(geometry)

target_sources(
  geometry
  PUBLIC FILE_SET
         CXX_MODULES
         FILES
           modules/geometry.cppm
           modules/matrix.cppm
           modules/matrix_view.cppm
           modules/primitives.cppm
           modules/triangle.cppm
)

target_compile_features(geometry PUBLIC cxx_std_23)
target_setup_for_install(geometry)
target_allow_instrumentation(geometry)
target_allow_coding_standards(geometry)

target_link_libraries(geometry PRIVATE utils)
add_subdirectory(tests)
