#pragma once

#include <graph.hpp>

namespace GraphUtils {

class GraphGenerator {
public:
    struct Parameters {
        float edge_probability;
        size_t vertex_count;
    };

    GraphGenerator(const Parameters& parameters);

    std::unique_ptr<GraphUtils::IGraph> generateGraph() const;

private:
    Parameters graph_parameters_;
};

} // GraphUtils
