#include <unordered_map>
#include <algorithm>

#include <sort.hpp>

#include "BridgeFinder.hpp"

class DeterminedOneBridgeVisitor : public Visitor {
public:
    DeterminedOneBridgeVisitor(const IGraph& graph)
    : m_graph(graph)
    , m_time(0) {
        m_earliest_node.resize(graph.getVertexCount());
        m_discovery_time.resize(graph.getVertexCount());
        m_parent.resize(graph.getVertexCount(), graph.getVertexCount());
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

    std::vector<bool> m_visited;
private:
    const IGraph& m_graph;

    size_t m_time;
    std::vector<size_t> m_earliest_node;
    std::vector<size_t> m_discovery_time;
    std::vector<Vertex> m_parent;
};

std::vector<Edge>  findOneBridgeFast(const IGraph& graph) {
    DeterminedOneBridgeVisitor visitor(graph);
    visitAll(graph, visitor);

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
    using EdgeList = std::vector<std::unordered_map<Vertex, uint32_t>>;

    RandomizedBridgeVisitor(const IGraph& graph) : m_graph(graph) {
        edges.resize(m_graph.getVertexCount());

        for (size_t vertex = 0; vertex < graph.getVertexCount(); ++vertex) {
            for (const auto neighbour : graph.getNeighbours(vertex)) {
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

    std::vector<WeightedEdge> getEdges() const {
        std::vector<WeightedEdge> edges_list;
        edges_list.reserve(m_graph.getEdgeCount());

        for (size_t from = 0; from < m_graph.getVertexCount(); ++from) {
            auto& neighbours = m_graph.getNeighbours(from);
            for (auto to : neighbours) {
                if (from < to) {
                    edges_list.push_back({{from, to}, this->getEdgeValue(from, to)});
                }
            }
        }
        return std::move(edges_list);
    }

    uint32_t& getEdgeValue(const Vertex& from, const Vertex& to) {
        return (from < to ? edges[from][to] : edges[to][from]); // TODO: potentialy dangerous
    }

    size_t getEdgeValue(const Vertex& from, const Vertex& to) const {
        return (from < to ? edges[from][to] : edges[to][from]);
    }

private:
    void setEdgeValue(const Vertex& from, const Vertex& to, size_t value) {
        getEdgeValue(from, to) = value;
    }

    mutable RandomizedBridgeVisitor::EdgeList edges; // edge representation works only with unordered graphs
    const IGraph& m_graph;
};

std::vector<Edge> findOneBridgeRandomized(const IGraph& graph) {
    assert(graph.getVertexCount() != 0);

    RandomizedBridgeVisitor visitor(graph);
    // dfs(graph, 0, visitor);
    visitAll(graph, visitor);

    auto edges = visitor.getEdges();

    std::vector<Edge> bridges;
    bridges.reserve(edges.size());
    for (const auto& edge : edges) {
        if (edge.weight == 0) {
            bridges.push_back({edge.edge});
        }
    }

    return bridges;
}

std::vector<std::vector<Edge>> findTwoBridgeRandomized(const IGraph& graph) {
    assert(graph.getVertexCount() != 0);

    RandomizedBridgeVisitor visitor(graph);
    visitAll(graph, visitor);

    auto edges = visitor.getEdges();
    assert(!edges.empty());
    std::vector<std::vector<Edge>> bridges;
    std::cout << edges.size() << std::endl;
// TODO: compare with different sort functions
    std::sort(edges.begin(), edges.end(), [&visitor](const WeightedEdge& left, const WeightedEdge& right)
    {return left.weight < right.weight;});
//    Utils::radix_sort(edges, [&visitor](const WeightedEdge& edge) {return edge.weight;});

    bridges.reserve(edges.size());
    bridges.push_back({edges[0].edge});
    size_t current_value = edges[0].weight;
    for (size_t i = 1; i < edges.size(); ++i) {
        const auto& edge = edges[i];
        if (edge.weight == current_value) {
            bridges.back().push_back(edge.edge);
        } else {
            bridges.push_back({{edge.edge}});
            current_value = edge.weight;
        }
    }

    bridges.erase(std::remove_if(bridges.begin(), bridges.end(), [](const std::vector<Edge>& edges) {return edges.size() < 2;}), bridges.end());
    return bridges;
}
