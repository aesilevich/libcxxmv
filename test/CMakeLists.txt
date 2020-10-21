
add_executable(cxxmvtest
               test.cpp
               vector_test.cpp)
target_link_libraries(cxxmvtest PRIVATE cxxmv Boost::boost)
