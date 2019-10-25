#include <algorithm>
#include <set>

#include "common.hpp"

using namespace GraphUtils;

TestGraph::TestGraph(const std::vector<std::vector<GraphUtils::Vertex>>& list) : m_list(list){
    for (GraphUtils::Vertex vertex = 0; vertex < m_list.size(); ++vertex) {
        auto& neighbours = getNeighbours(vertex);
        std::vector<GraphUtils::Vertex> missed;
        for (const GraphUtils::Vertex& neighbour : neighbours) {
            if (std::find(getNeighbours(neighbour).begin(), getNeighbours(neighbour).end(), vertex) == getNeighbours(neighbour).end()) {
                m_list[neighbour].push_back(vertex);
            }
        }
    }
}

const std::vector<GraphUtils::Vertex>& TestGraph::getNeighbours(GraphUtils::Vertex vertex) const {
    assert(vertex < getVertexCount());
    return m_list[vertex];
}

size_t TestGraph::getVertexCount() const {
    return m_list.size();
}
