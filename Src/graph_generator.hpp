#pragma once

#include <graph.hpp>

namespace GraphUtils {



class GraphGenerator {
    struct GraphParametes {
        size_t min_vertex_count;
        size_t max_vertex_count;
        size_t min_neighbours_count;
        size_t max_neighbours_count;
    };

public:
    GraphGenerator(GraphParametes);

    std::unique_ptr<GraphUtils::UndirectedGraph> generateGraph() const;

private:
    GraphParametes graph_parameters_;
};

} // GraphUtils
