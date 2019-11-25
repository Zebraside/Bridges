#pragma once

#include <graph.hpp>

namespace GraphUtils {

class GraphGenerator {
    struct GraphParametes {
        float edge_probability;
        size_t vertex_count;
    };

public:
    GraphGenerator(GraphParametes parameters);

    std::unique_ptr<GraphUtils::IGraph> generateGraph() const;

private:
    GraphParametes graph_parameters_;
};

} // GraphUtils
