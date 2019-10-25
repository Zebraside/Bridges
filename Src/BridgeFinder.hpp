#pragma once
#include "graph.hpp"

using namespace GraphUtils;

std::vector<Edge> findOneBridgeFast(const UndirectedGraph& graph);
std::vector<Edge> findOneBridgeRandomized(const UndirectedGraph& graph);
std::vector<std::vector<Edge>> findTwoBridgeRandomized(const UndirectedGraph& graph);
