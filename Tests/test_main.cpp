#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <bencmark.hpp>



TEST_CASE("Can measure time", "[benchmark]" ) {
    auto sleep_for = [](int sec) {
        std::cout << "sleep for " << sec << std::endl;
        sleep(sec);
    };

    auto execution_time = Benchmark::measureExecutionTime(sleep_for, 1);

    std::cout << execution_time << std::endl;
    REQUIRE(std::abs(execution_time - 1.0) < 0.001);
}

int g_func() {
    sleep(10); return 42;
}

TEST_CASE("Test async", "[benchmark]" ) {

}
