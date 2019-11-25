#include <stack>
#include <set>
#include <algorithm>

#include "graph.hpp"

namespace GraphUtils {

void dfsStackImpl(const Vertex& root, const Vertex& /*parent*/ , std::vector<bool>& visited, const IGraph& graph, Visitor& visitor) {
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

void dfsRecursiveImpl(const Vertex& root, const Vertex& parent, std::vector<bool>& visited, const IGraph& graph, Visitor& visitor) {
    visited[root] = true;
    visitor.onEnter(root, parent);

    for (const auto& neighbour : graph.getNeighbours(root)) {
        if (!visited[neighbour]) {
            dfsRecursiveImpl(neighbour, root, visited, graph, visitor);
        }
    }

    visitor.onLeave(root, parent);
}

void dfs(const IGraph& graph, const Vertex& start, Visitor& visitor) {
    std::vector<bool> visited(graph.getVertexCount());
    dfsStackImpl(start, graph.getVertexCount() + 1, visited, graph, visitor);
}

// TODO: may cause performance problems
const std::vector<Vertex>& UndirectedGraph::getNeighbours(Vertex vertex) const {
    assert(vertex < adjacency_list_.size());
    return adjacency_list_[vertex];
}

size_t UndirectedGraph::getVertexCount() const {
    return adjacency_list_.size();
}

UndirectedGraph::UndirectedGraph(std::vector<Edge> edge_list) {

    for (const auto& [u, v] : edge_list) {
        if (std::max(u,v) + 1 >= adjacency_list_.size())
            adjacency_list_.resize(std::max(u,v) + 1);

        if (std::find(adjacency_list_[u].begin(), adjacency_list_[u].end(), v) == adjacency_list_[u].end()) {
            adjacency_list_[u].push_back(v);
        }

        if (std::find(adjacency_list_[v].begin(), adjacency_list_[v].end(), u) == adjacency_list_[v].end()) {
            adjacency_list_[v].push_back(u);
        }
    }
}

size_t UndirectedGraph::getEdgeCount() const {
    size_t edge_count = 0;
    for (size_t vertex = 0; vertex < getVertexCount(); ++vertex) {
        edge_count += getNeighbours(vertex).size();
    }

    return edge_count / 2;
}
} // GraphUtils
