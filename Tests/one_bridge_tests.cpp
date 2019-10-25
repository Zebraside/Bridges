#include <set>

#include <catch.hpp>

#include <graph.hpp>
#include <BridgeFinder.hpp>
#include <map>

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

TEST_CASE("Randomized one bridge works", "two_randomized") {
    for (const auto& data : test_data) {
        TestGraph g(data.input);
        auto bridges = findOneBridgeRandomized(g);

        REQUIRE(Equal(data.expected, bridges));
    }
}

TEST_CASE("Determined one bridge works", "two_randomized") {
    for (const auto& data : test_data) {
        TestGraph g(data.input);
        auto bridges = findOneBridgeFast(g);

        REQUIRE(Equal(data.expected, bridges));
    }
}

