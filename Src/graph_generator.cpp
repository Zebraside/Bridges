#include <set>
#include <algorithm>

#include <random_generator.hpp>

#include "graph_generator.hpp"

namespace GraphUtils {

class BaseGraph :  public UndirectedGraph {
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

GraphGenerator::GraphGenerator(GraphParametes parameters) : graph_parameters_(parameters) {
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

std::unique_ptr<GraphUtils::UndirectedGraph> GraphGenerator::generateGraph() const {
    Utils::FixedPointGenerator<size_t> generator;
    size_t vertex_count = generator.generateValue(graph_parameters_.min_vertex_count, graph_parameters_.max_vertex_count);
    std::vector<std::vector<Vertex>> adjacency_list(vertex_count);
    for (size_t i = 0; i < vertex_count - 1; ++i) {
        size_t neighbours_count = generator.generateValue(graph_parameters_.min_neighbours_count, graph_parameters_.max_neighbours_count);
        adjacency_list[i] = generateRandomUniqueSequence(neighbours_count, i + 1, vertex_count - 1);
    }

    for (size_t i = 0; i < vertex_count; ++i) {
        for (const auto& n : adjacency_list[i]) {
            adjacency_list[n].push_back(i);
        }
    }

    for (size_t i = 0; i < vertex_count; ++i) {
        adjacency_list[i] = onlyUnique(adjacency_list[i]);
    }

    assert(std::all_of(adjacency_list.begin(), adjacency_list.end(), [](const std::vector<Vertex> neighbours){return !neighbours.empty();}));

    return std::unique_ptr<UndirectedGraph>(new BaseGraph(adjacency_list));
}

} // GraphUtils
