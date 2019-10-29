
add_library(Graph graph.cpp graph.hpp BridgeFinder.cpp graph_generator.cpp)
target_link_libraries(Graph PUBLIC Utils)
target_include_directories(Graph PUBLIC ${CMAKE_SOURCE_DIR}/Src)

add_executable(Bridges main.cpp)

target_link_libraries(Bridges Graph)
