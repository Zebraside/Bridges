#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <bencmark.hpp>
#include <graph_generator.hpp>
#include <BridgeFinder.hpp>

//TEST_CASE("Can measure time", "[benchmark]" ) {
//    auto sleep_for = [](int sec) {
//        std::cout << "sleep for " << sec << std::endl;
//        sleep(sec);
//    };
//
//    auto execution_time = Benchmark::measureExecutionTime(sleep_for, 1);
//
//    std::cout << execution_time << std::endl;
//    REQUIRE(std::abs(execution_time - 1.0) < 0.001);
//}
//
//TEST_CASE("Test on random graph", "[benchmark]" ) {
//    GraphUtils::GraphGenerator graph_generator({5, 150, 5, 10});
//    auto graph = graph_generator.generateGraph();
//    auto bridges = findOneBridgeRandomized(*graph);
//    bridges = findOneBridgeFast(*graph);
//    auto two_bridges = findTwoBridgeRandomized(*graph);
//    int x = 42;
//
//}
