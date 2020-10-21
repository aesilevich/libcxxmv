

add_library(cxxmv INTERFACE)
target_include_directories(cxxmv INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(cxxmv INTERFACE Boost::boost)

add_subdirectory(qt)
