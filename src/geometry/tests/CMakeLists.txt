add_executable(geometry_tests)

target_link_libraries(geometry_tests PRIVATE geometry)
target_link_libraries(geometry_tests PRIVATE Boost::headers)

target_sources(geometry_tests PRIVATE test.cpp)

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
