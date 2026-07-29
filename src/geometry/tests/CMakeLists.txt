add_executable(geometry_tests)

target_link_libraries(geometry_tests PRIVATE geometry)
target_link_libraries(geometry_tests PRIVATE Boost::headers)

target_sources(geometry_tests PRIVATE test.cpp)

# In gcc 15 libstdc++ (used by the nixpkgs 26.05 Clang), `std::array::operator[]` runs a bounds-check assertion that is
# also evaluated during constant evaluation, so the compile-time 6x6 matrix inversion in test.cpp overruns Clang's
# default step budget. Raise it to match gcc's default `-fconstexpr-ops-limit`. Scoped to this test executable, which is
# never installed, so the flag cannot leak into consumers of the exported modules.
target_compile_options(geometry_tests PRIVATE $<$<CXX_COMPILER_ID:Clang>:-fconstexpr-steps=33554432>)

target_allow_instrumentation(geometry_tests)
target_allow_coding_standards(geometry_tests)

add_tests(geometry_tests)

add_executable(geom_triangle_tests)

target_link_libraries(geom_triangle_tests PRIVATE geometry)
target_link_libraries(geom_triangle_tests PRIVATE Boost::headers)

target_sources(geom_triangle_tests PRIVATE triangle.cpp)

target_allow_instrumentation(geom_triangle_tests)
target_allow_coding_standards(geom_triangle_tests)

add_tests(geom_triangle_tests)
