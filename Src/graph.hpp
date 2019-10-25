#pragma once
#include <cstdlib>
#include <utility>
#include <vector>
#include <memory>
#include <cassert>

#include <iostream>

namespace GraphUtils {

using Vertex = size_t;
using Edge = std::pair<Vertex, Vertex>;

class UndirectedGraph {
public:
    virtual const std::vector<Vertex>& getNeighbours(Vertex vertex) const = 0;
    virtual size_t getVertexCount() const = 0;
    virtual ~UndirectedGraph() = default;
};

struct Visitor {
    virtual void onEnter(const Vertex& root, const Vertex& parent) = 0;
    virtual void onLeave(const Vertex& root, const Vertex& parent) = 0;
};

void dfs(const UndirectedGraph& graph, const Vertex& start, Visitor& visitor);

} // GraphUtils
