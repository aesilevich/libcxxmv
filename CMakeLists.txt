project(libcxxmv)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

include(FindBoost)
find_package(Boost 1.64 REQUIRED) 

add_subdirectory(cxxmv)
add_subdirectory(examples)
add_subdirectory(test)
