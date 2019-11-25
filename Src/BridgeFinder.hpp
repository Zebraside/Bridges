#pragma once
#include "graph.hpp"

using namespace GraphUtils;

std::vector<Edge> findOneBridgeFast(const IGraph& graph);
std::vector<Edge> findOneBridgeRandomized(const IGraph& graph);
std::vector<std::vector<Edge>> findTwoBridgeRandomized(const IGraph& graph);
