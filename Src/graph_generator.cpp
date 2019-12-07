#include <set>
#include <algorithm>

#include <random_generator.hpp>

#include "graph_generator.hpp"

namespace GraphUtils {

class BaseGraph :  public IGraph {
public:
    BaseGraph(std::vector<std::vector<Vertex>> adjacency_list) : m_list(adjacency_list) {
    }

    const std::vector<Vertex> &getNeighbours(Vertex vertex) const override {
        assert(vertex < m_list.size());
        return m_list[vertex];
    }

    size_t getVertexCount() const override {
        return m_list.size();
    }

private:
    std::vector<std::vector<Vertex>> m_list;
};

GraphGenerator::GraphGenerator(const Parameters& parameters) : graph_parameters_(parameters) {
}

static std::vector<size_t> generateRandomUniqueSequence(size_t max_size, size_t min_idx, size_t max_idx) {
    std::set<size_t> sequence;
    Utils::FixedPointGenerator<size_t> generator;
    for (size_t i = 0; i < max_size; ++i) {
        sequence.insert(generator.generateValue(min_idx, max_idx));
    }

    return std::vector<size_t>(sequence.begin(), sequence.end());
}

static std::vector<Vertex> onlyUnique(const std::vector<Vertex>& vertexes) {
    std::set<Vertex> unique(vertexes.begin(), vertexes.end());
    return std::vector<Vertex>(unique.begin(), unique.end());
}

std::unique_ptr<GraphUtils::IGraph> GraphGenerator::generateGraph() const {
    Utils::FlaotingPointGenerator<float> generator;

    std::vector<GraphUtils::Edge > edge_list;
    edge_list.reserve(graph_parameters_.vertex_count * graph_parameters_.vertex_count / 2.0 * graph_parameters_.edge_probability);
    for (size_t u = 0; u < graph_parameters_.vertex_count; ++u) {
        for (size_t v = u + 1; v < graph_parameters_.vertex_count; ++v) {
            auto prob = generator.generateValue(0.0f, 1.f);
            if (prob < graph_parameters_.edge_probability) {
                if (edge_list.size() + 10 > edge_list.max_size()) {
                    int x = 42;
                }
                
                edge_list.push_back({u, v});
            }
        }
    }

    return std::unique_ptr<IGraph>(new UndirectedGraph(edge_list));
}

} // GraphUtils
