#include <set>

#include <catch.hpp>

#include <graph.hpp>
#include <BridgeFinder.hpp>

#include "common.hpp"

using namespace GraphUtils;

static bool Equal(std::vector<std::vector<Edge>> expected, std::vector<std::vector<Edge>> actual) {
    if (expected.size() != actual.size()) {
        std::cout << "Expected and actual size mismatch" << std::endl;
        return false;
    }

    for (size_t i = 0; i < actual.size(); ++i) {
        std::sort(actual[i].begin(), actual[i].end());
        std::sort(expected[i].begin(), expected[i].end());
    }

    auto comparator = [](const std::vector<Edge>& left, const std::vector<Edge>& right) {
        if (left.size() < right.size())
            return true;

        if (left.size() == right.size()) {
            for (size_t i = 0; i < left.size(); ++i) {
                if (left[i] == right[i])
                    continue;

                return left[i] < right[i];
            }
        }

        return false;
    };

    std::sort(actual.begin(), actual.end(), comparator);
    std::sort(expected.begin(), expected.end(), comparator);

    for (size_t i = 0; i < actual.size(); ++i) {
        if (expected[i].size() != actual[i].size())
            return false;

        for (size_t j = 0; j < actual[i].size(); ++j) {
            if (actual[i][j] != expected[i][j])
                return false;
        }
    }

    return true;
}

TEST_CASE("Two bridges works for non splittable", "randomized") {
    std::vector<std::vector<Vertex>> l = {
            {1, 2, 3},
            {2, 3},
            {3},
            {}};

    TestGraph g(l);
    auto bridge_groups = findTwoBridgeRandomized(g);
    REQUIRE(bridge_groups.empty());
}

TEST_CASE("Two bridges works for single edge", "randomized" ) {
    std::vector<std::vector<Vertex>> l = {{1}, {}};
    TestGraph g(l);
    auto bridge_groups = findTwoBridgeRandomized(g);
    REQUIRE(bridge_groups.empty());
}

TEST_CASE("Two bridges works for only variant", "randomized" ) {
    std::vector<std::vector<Vertex>> l = {
            {1, 2},
            {2, 3, 4},
            {3, 4},
            {4},
            {}};

    TestGraph g(l);
    auto bridge_groups = findTwoBridgeRandomized(g);
    REQUIRE(!bridge_groups.empty());

    std::vector<std::vector<Edge>> expected = {{{0, 1}, {0, 2}}};
    REQUIRE(Equal(expected, bridge_groups));
}

TEST_CASE("Two bridges works for multiple variants", "randomized" ) {
    std::vector<std::vector<Vertex>> l = {
            {1, 2},
            {2},
            {}};

    TestGraph g(l);
    auto bridge_groups = findTwoBridgeRandomized(g);
    REQUIRE(!bridge_groups.empty());

    std::vector<std::vector<Edge>> expected = {{{0,1}, {1, 2}, {0, 2}}};
    REQUIRE(Equal(expected, bridge_groups));
}
