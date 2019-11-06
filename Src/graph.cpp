#include <stack>
#include <set>

#include "graph.hpp"

namespace GraphUtils {

void dfsStackImpl(const Vertex& root, const Vertex& /*parent*/ , std::vector<bool>& visited, const UndirectedGraph& graph, Visitor& visitor) {
    std::stack<Vertex> vertex_to_visit;
    vertex_to_visit.push(root);
    std::vector<bool> exited(graph.getVertexCount());

    std::vector<Vertex> parent(graph.getVertexCount(), graph.getVertexCount());


    while (!vertex_to_visit.empty()) {
        const auto& current_vertex = vertex_to_visit.top();

        if (!visited[current_vertex]) {
            visited[current_vertex] = true;
            visitor.onEnter(current_vertex, parent[current_vertex]);

            for (const auto &neighbour : graph.getNeighbours(current_vertex)) {
                if (!visited[neighbour]) {
                    parent[neighbour] = current_vertex;
                    vertex_to_visit.push(neighbour);
                }
            }
        } else if (!exited[current_vertex]) {
            exited[current_vertex] = true;
            vertex_to_visit.pop();
            visitor.onLeave(current_vertex, parent[current_vertex]);
        } else {
            vertex_to_visit.pop();
        }
    }
}

void dfsRecursiveImpl(const Vertex& root, const Vertex& parent, std::vector<bool>& visited, const UndirectedGraph& graph, Visitor& visitor) {
    visited[root] = true;
    visitor.onEnter(root, parent);

    for (const auto& neighbour : graph.getNeighbours(root)) {
        if (!visited[neighbour]) {
            dfsRecursiveImpl(neighbour, root, visited, graph, visitor);
        }
    }

    visitor.onLeave(root, parent);
}

void dfs(const UndirectedGraph& graph, const Vertex& start, Visitor& visitor) {
    std::vector<bool> visited(graph.getVertexCount());
    dfsStackImpl(start, graph.getVertexCount() + 1, visited, graph, visitor);
}

} // GraphUtils
