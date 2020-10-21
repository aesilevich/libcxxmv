
add_executable(line_view_example main.cpp)
target_compile_options(line_view_example PRIVATE -fconcepts-diagnostics-depth=20)
target_link_libraries(line_view_example PRIVATE cxxmv-qt)
