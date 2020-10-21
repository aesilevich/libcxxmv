
find_package(Qt5 COMPONENTS "Widgets")

add_library(cxxmv-qt STATIC
            cxxmvqt.cpp
            line_view.cpp
            line_view.hpp)
target_compile_options(cxxmv-qt PRIVATE -fconcepts-diagnostics-depth=2)

target_link_libraries(cxxmv-qt PUBLIC cxxmv Qt5::Widgets)
