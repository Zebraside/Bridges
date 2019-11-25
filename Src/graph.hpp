#pragma once
#include <cstdlib>
#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include <unordered_set>

#include <iostream>

namespace GraphUtils {

using Vertex = size_t;
using Edge = std::pair<Vertex, Vertex>;

class IGraph {
public:
    virtual const std::vector<Vertex>& getNeighbours(Vertex vertex) const = 0;
    virtual size_t getVertexCount() const = 0;
    virtual size_t getEdgeCount() const = 0;
    virtual ~IGraph() = default;
};

class UndirectedGraph : public IGraph {
public:
    UndirectedGraph(std::vector<Edge> edge_list);
    const std::vector<Vertex> &getNeighbours(Vertex vertex) const override;
    size_t getVertexCount() const override;

    size_t getEdgeCount() const override;

private:
    std::vector<std::vector<Vertex>> adjacency_list_;
};

struct Visitor {
    virtual void onEnter(const Vertex& root, const Vertex& parent) = 0;
    virtual void onLeave(const Vertex& root, const Vertex& parent) = 0;
};

void dfs(const IGraph& graph, const Vertex& start, Visitor& visitor);

} // GraphUtils
