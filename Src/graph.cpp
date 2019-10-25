#include "graph.hpp"

namespace GraphUtils {

// TODO: make non recursive implementation
void dfsImpl(const Vertex& root, const Vertex& parent, std::vector<bool>& visited, const UndirectedGraph& graph, Visitor& visitor) {
    visited[root] = true;
    visitor.onEnter(root, parent);

    for (const auto& neighbour : graph.getNeighbours(root)) {
        if (!visited[neighbour]) {
            dfsImpl(neighbour, root, visited, graph, visitor);
        }
    }

    visitor.onLeave(root, parent);
}

void dfs(const UndirectedGraph& graph, const Vertex& start, Visitor& visitor) {
    std::vector<bool> visited(graph.getVertexCount());
    dfsImpl(start, graph.getVertexCount() + 1, visited, graph, visitor);
}

} // GraphUtils
