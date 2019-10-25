#pragma once

#include <graph.hpp>

class TestGraph : public GraphUtils::UndirectedGraph {
public:
    TestGraph(const std::vector<std::vector<GraphUtils::Vertex>>& list);

    const std::vector<GraphUtils::Vertex>& getNeighbours(GraphUtils::Vertex vertex) const override;
    size_t getVertexCount() const override;

private:
    std::vector<std::vector<GraphUtils::Vertex>> m_list;
};
