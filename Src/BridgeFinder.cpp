#include <unordered_map>
#include <algorithm>

#include "BridgeFinder.hpp"

class DeterminedOneBridgeVisitor : public Visitor {
public:
    DeterminedOneBridgeVisitor(const UndirectedGraph& graph)
    : m_graph(graph)
    , m_time(0) {
        m_earliest_node.resize(graph.getVertexCount());
        m_discovery_time.resize(graph.getVertexCount());
        m_parent.resize(graph.getVertexCount());
    }

    void onEnter(const Vertex& root, const Vertex& parent) override {
        m_discovery_time[root] = m_earliest_node[root] = m_time++;
        m_parent[root] = parent;
    }

    void onLeave(const Vertex& root, const Vertex& parent) override {
        if (parent < m_graph.getVertexCount()) {
            m_earliest_node[parent] = std::min(m_earliest_node[root], m_earliest_node[parent]);
        }

        for (auto& neighbour : m_graph.getNeighbours(root)) {
            if (neighbour == parent) {
                continue;
            }

            m_earliest_node[root] = std::min(m_earliest_node[root], m_earliest_node[neighbour]);
            m_earliest_node[root] = std::min(m_earliest_node[root], m_discovery_time[neighbour]);
        }
    }

    const std::vector<size_t>& getEarliestNode() const {
        return m_earliest_node;
    }

    const std::vector<size_t>& getDiscoveryTime() const {
        return m_discovery_time;
    }

    const std::vector<Vertex>& getParents() const {
        return m_parent;
    }

private:
    const UndirectedGraph& m_graph;

    size_t m_time;
    std::vector<size_t> m_earliest_node;
    std::vector<size_t> m_discovery_time;
    std::vector<Vertex> m_parent;
};

std::vector<Edge>  findOneBridgeFast(const UndirectedGraph& graph) {
    DeterminedOneBridgeVisitor visitor(graph);
    dfs(graph, 0, visitor);

    auto& earliest_nodes = visitor.getEarliestNode();
    auto& discovery_time = visitor.getDiscoveryTime();
    auto& parents = visitor.getParents();
    std::vector<Edge> bridges;
    for (size_t vertex = 0; vertex < graph.getVertexCount(); ++vertex) {
        for (const auto& neighbour : graph.getNeighbours(vertex)) {
            if (earliest_nodes[neighbour] > discovery_time[vertex] && parents[vertex] != neighbour) {
                Edge edge = vertex < neighbour ? Edge{vertex, neighbour} : Edge{neighbour, vertex};
                bridges.push_back(edge);
            }
        }
    }

    return bridges;
}

// Assumption: access to edges via pairs like (v1, v2) where v1 < v2
class RandomizedBridgeVisitor : public Visitor {
public:
    using EdgeList = std::unordered_map<size_t, std::unordered_map<size_t, size_t>>;

    RandomizedBridgeVisitor(const UndirectedGraph& graph) : m_graph(graph) {
        for (size_t vertex = 0; vertex < graph.getVertexCount(); ++vertex) {
            for (const auto& neighbour : graph.getNeighbours(vertex)) {
                assert(vertex != neighbour);
                setEdgeValue(vertex, neighbour, std::rand()); // TODO: generate normal random value
            }
        }

        // TODO: check that all edges are added
    }

    void onEnter(const Vertex& root, const Vertex& parent) override {

    }

    void onLeave(const Vertex& root, const Vertex& parent) override {
        if (parent >= m_graph.getVertexCount()) // All edges have already been processed
            return;

        size_t vertexSum = 0;
        for (const auto& vertex : m_graph.getNeighbours(root)) {
            if (vertex == parent) {
                continue;
            }

            vertexSum ^= getEdgeValue(root, vertex);
        }

        setEdgeValue(root, parent, vertexSum);
    }

    std::vector<Edge> getEdges() const {
        std::vector<Edge> edges_list;
        for (auto& [from, neighbours] : edges) {
            for (auto& [to, value] : neighbours) {
                if (from < to) {
                    edges_list.emplace_back(from, to);
                }
            }
        }
        return edges_list;
    }

    size_t& getEdgeValue(const Vertex& from, const Vertex& to) {
        return (from < to ? edges[from][to] : edges[to][from]); // TODO: potentialy dangerous
    }

private:
    void setEdgeValue(const Vertex& from, const Vertex& to, size_t value) {
        getEdgeValue(from, to) = value;
    }

//    static bool isUndirected(const UndirectedGraph& graph) {
//        for (size_t vertex = 0; vertex < graph.getVertexCount(); ++vertex) {
//            const auto& neighbours = graph.getNeighbours(vertex);
//            for (const auto& neighbour : neighbours) {
//                const auto& neighbour_neighbours = graph.getNeighbours(neighbour);
//                auto it = std::find(neighbour_neighbours.begin(), neighbour_neighbours.end(), vertex);
//                if (it == neighbour_neighbours.end())
//                    return false;
//            }
//        }
//
//        return true;
//    }

    std::unordered_map<size_t, std::unordered_map<size_t, size_t>> edges; // edge representation works only with unordered graphs
    const UndirectedGraph& m_graph;
};

std::vector<Edge> findOneBridgeRandomized(const UndirectedGraph& graph) {
    assert(graph.getVertexCount() != 0);

    RandomizedBridgeVisitor visitor(graph);
    dfs(graph, 0, visitor);

    std::vector<Edge> bridges;
    for (const auto& edge : visitor.getEdges()) {
        if (visitor.getEdgeValue(edge.first, edge.second) == 0) {
            bridges.push_back({edge.first, edge.second});
        }
    }

    return bridges;
}

std::vector<std::vector<Edge>> findTwoBridgeRandomized(const UndirectedGraph& graph) {
    assert(graph.getVertexCount() != 0);

    RandomizedBridgeVisitor visitor(graph);
    dfs(graph, 0, visitor);

    auto edges = visitor.getEdges();
    assert(!edges.empty());

    // TODO: replace with radix sort
    std::sort(edges.begin(), edges.end(), [&visitor](const Edge& left, const Edge& right)
    {return visitor.getEdgeValue(left.first, left.second) < visitor.getEdgeValue(right.first, right.second);});
    std::vector<std::vector<Edge>> bridges;

    bridges.push_back({edges[0]});
    size_t current_value = visitor.getEdgeValue(edges[0].first, edges[0].second);
    for (size_t i = 1; i < edges.size(); ++i) {
        const auto& edge = edges[i];
        if (visitor.getEdgeValue(edge.first, edge.second) == current_value) {
            bridges.back().push_back({edge.first, edge.second});
        } else {
            bridges.push_back({{{edge.first, edge.second}}});
            current_value = visitor.getEdgeValue(edge.first, edge.second);
        }
    }

    bridges.erase(std::remove_if(bridges.begin(), bridges.end(), [](const std::vector<Edge>& edges) {return edges.size() < 2;}), bridges.end());
    return bridges;
}
