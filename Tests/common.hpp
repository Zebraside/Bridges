#pragma once

#include <graph.hpp>

class TestGraph : public GraphUtils::IGraph {
public:
    TestGraph(const std::vector<std::vector<GraphUtils::Vertex>>& list);

    const std::vector<GraphUtils::Vertex>& getNeighbours(GraphUtils::Vertex vertex) const override;
    size_t getVertexCount() const override;

    size_t getEdgeCount() const override;

private:
    std::vector<std::vector<GraphUtils::Vertex>> m_list;
};

template <class T>
static bool Equal(const T& actual, const T& expected) {
    if (actual.size() != expected.size()) {
        return false;
    }

    for (size_t i = 0; i < actual.size(); ++i) {
        if (actual[i] != expected[i]) {
            return false;
        }
    }

    return true;
}
