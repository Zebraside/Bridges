#include <algorithm>

#include <catch.hpp>

#include <graph.hpp>

#include "common.hpp"

using namespace GraphUtils;

namespace {
class TestVisitor : public GraphUtils::Visitor {
public:
    TestVisitor(const IGraph& graph) {
        enter_count.resize(graph.getVertexCount());
        leave_count.resize(graph.getVertexCount());
    }

    void onEnter(const Vertex &root, const Vertex &parent) override {
        enter_count[root]++;
    }

    void onLeave(const Vertex &root, const Vertex &parent) override {
        leave_count[root]++;
    }

    std::vector<size_t> enter_count;
    std::vector<size_t> leave_count;

};
}

TEST_CASE("Dfs visit all nodes in connected graph only once", "[graph]" ) {
    std::vector<std::vector<std::vector<Vertex>>> tests = {
            {{1}, {}},
            {{1, 2}, {}, {}},
            {{1, 2}, {1}, {}}
    };

    for (const auto& test : tests) {
        TestGraph test_graph(test);
        TestVisitor visitor(test_graph);

        dfs(test_graph, 0, visitor);

        REQUIRE(std::all_of(visitor.enter_count.begin(), visitor.enter_count.end(), [](size_t value){return value == 1;}));
        REQUIRE(std::all_of(visitor.leave_count.begin(), visitor.leave_count.end(), [](size_t value){return value == 1;}));
    }
}
