#include <set>
#include <map>

#include <catch.hpp>

#include <graph.hpp>
#include <BridgeFinder.hpp>
#include <graph_generator.hpp>
#include <bencmark.hpp>

#include "common.hpp"

using namespace GraphUtils;
using UndirectedAdjacencyList = std::vector<std::vector<Vertex>>;
struct TestResult {
    UndirectedAdjacencyList input;
    std::vector<Edge> expected;
};

bool haveDuplications(const std::vector<GraphUtils::Edge>& actual) {
    std::set<Edge> unique_edges(actual.begin(), actual.end());
    return actual.size() == unique_edges.size();
}

bool Equal(const  std::vector<Edge>& expected, const std::vector<Edge>& actual) {
    if (expected.size() != actual.size()) {
        std::cout << "Expected and actual size mismatch\n";
        return false;
    }

    for (size_t i = 0; i < expected.size(); ++i) {
        if (std::find(actual.begin(), actual.end(), expected[i]) == actual.end()) {
            std::cout << "Not all expected edges where found\n";
            return false;
        }
    }

    return true;
}

std::vector<TestResult> test_data = {
        {{{1}, {2}, {0}}, {}}, // not splittable
        {{{1}, {}}, {{0, 1}}}, // single edge
        {{{1, 2}, {2}, {3}, {4, 5}, {5}, {}}, {{2, 3}}}, // single bridge
        {{{1, 2, 3, 4}, {}, {}, {}, {}}, {{0, 1}, {0, 2}, {0, 3}, {0, 4}}} // multiple bridges
};

TEST_CASE("Randomized one bridge works", "one_randomized") {
    for (const auto& data : test_data) {
        TestGraph g(data.input);
        auto bridges = findOneBridgeRandomized(g);

        REQUIRE(Equal(data.expected, bridges));
    }
}

TEST_CASE("Determined one bridge works", "one_determined") {
    for (const auto& data : test_data) {
        TestGraph g(data.input);
        auto bridges = findOneBridgeFast(g);

        REQUIRE(Equal(data.expected, bridges));
    }
}

TEST_CASE("Stress cross test") {
    GraphUtils::GraphGenerator graph_generator({1000, 5000, 1, 48});

    for (size_t i = 0; i < 100; ++i) {
        auto graph = graph_generator.generateGraph();
        auto randomized = findOneBridgeRandomized(*graph);
        auto determined = findOneBridgeFast(*graph);

        REQUIRE(Equal(determined, randomized));
    }
}

TEST_CASE("Bench") {
    GraphUtils::GraphGenerator graph_generator({13000, 15000, 1, 48});

    for (size_t i = 0; i < 100; ++i) {
        auto graph = graph_generator.generateGraph();
        std::cout << graph->getVertexCount() << std::endl;
        std::vector<Edge> randomized;
        std::vector<Edge> determined;

        auto f = [&]() {
            randomized = findOneBridgeRandomized(*graph);
            determined = findOneBridgeFast(*graph);
        };
        auto exec_time = Benchmark::measureExecutionTime(f);

        REQUIRE(Equal(determined, randomized));
    }
}

