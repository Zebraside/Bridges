#include <fstream>
#include "graph.hpp"
#include "BridgeFinder.hpp"
#include "graph_generator.hpp"
#include "bencmark.hpp"

constexpr float getDesiredProbability(size_t vertex_count, size_t need_edge_count) {
    assert(vertex_count > 0);
    double probability = (2 * need_edge_count) / vertex_count;
    probability /= vertex_count - 1;

    return probability;
}

std::vector<GraphGenerator::Parameters> prepareParametersList() {
    std::vector<size_t> vertex_count = {/*10, 100, 1'000, 10'000, 50'000, */100'000};
    std::vector<float> density = {/*0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8,*/ 0.9};

    std::vector<GraphGenerator::Parameters> parameters;
    for (auto vertex : vertex_count) {
        for (auto d : density) {
            auto edge_count = d * ((vertex * vertex) / 2);
            parameters.push_back({getDesiredProbability(vertex, edge_count), vertex});
        }
    }

    return parameters;
}

std::vector<GraphGenerator::Parameters> generation_parameters = {
    {getDesiredProbability(10000, 10000), 10000},
    {getDesiredProbability(10000, 100000), 10000},
    {getDesiredProbability(10000, 500000), 10000},
    {getDesiredProbability(100000, 100000), 100000},
    {getDesiredProbability(100000, 1000000), 100000},
    {getDesiredProbability(100000, 5000000), 100000},
};

struct Result {
    GraphGenerator::Parameters graph_parameters;
    double average_edge_count;
    double one_fast_average_time;
    double one_random_average_time;
    double two_random_average_time;
};

void logResult(const std::string& result_path, const Result& result) {
    std::fstream ofs(result_path, std::ios::out | std::fstream::app);

    ofs << result.graph_parameters.vertex_count << " " << result.average_edge_count << " " << result.graph_parameters.edge_probability << std::endl;
    ofs << result.one_fast_average_time << " " << result.one_random_average_time << std::endl;
    ofs << result.two_random_average_time << std::endl;

    ofs.close();
}

#include <future>
#include <mutex>

int main() {
    auto parameters = prepareParametersList();
    for (const auto& parameter : parameters) {
        std::cout << parameter.vertex_count << " " << parameter.edge_probability << std::endl;
	size_t experiment_count = 1;

        double one_random_average_time = 0.0;
        double one_fast_average_time = 0.0;
        double two_random_average_time = 0.0;

        double average_edge_count = 0;
        for (size_t i = 0; i < experiment_count; ++i) {
            GraphUtils::GraphGenerator graph_generator(parameter);

            auto graph = graph_generator.generateGraph();
            average_edge_count += graph->getEdgeCount() / experiment_count;

            auto one_randomized = [&]() {findOneBridgeRandomized(*graph);};
            auto one_fast = [&]() {findOneBridgeFast(*graph);};
            auto two_randomized = [&]() {findTwoBridgeRandomized(*graph);};

            auto one_bridge_randomized_time = Benchmark::measureExecutionTime(one_randomized);
            one_random_average_time += one_bridge_randomized_time / experiment_count;

            auto one_bridge_determined_time = Benchmark::measureExecutionTime(one_fast);
            one_fast_average_time += one_bridge_determined_time / experiment_count;

            auto two_bridge_randomized_time = Benchmark::measureExecutionTime(two_randomized);
            two_random_average_time += two_bridge_randomized_time / experiment_count;
        }

        logResult("/home/kmolchan/Private/Bridges/radix_stack_64.txt", {parameter, average_edge_count, one_fast_average_time, one_random_average_time, two_random_average_time});
    }
    return 0;
}
