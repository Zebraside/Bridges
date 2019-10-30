#include <iostream>

#include <catch.hpp>

#include <sort.hpp>
#include <random_generator.hpp>

#include <common.hpp>

struct TestStruct {
    size_t idx;
    size_t value;

};

bool operator!= (const TestStruct& left, const TestStruct& right) {
    return left.idx != right.idx || left.value != right.value;
}

bool operator< (const TestStruct& left, const TestStruct& right) {
    return left.value < right.value;
}

TEST_CASE("Radix simple test") {
    constexpr size_t min = std::numeric_limits<size_t>::min();
    constexpr size_t max = std::numeric_limits<size_t>::max();
    std::vector<std::vector<size_t>> simple_tests = {
            {5, 4, 3, 2, 1},
            {1, 2, 3, 4, 5},
            {1, 1, 1, 1, 1},
            {0, 1, 0, 1, 0},
            {},
            {1},
            {1, 32},
            {23, 1},
            {min, max},
            {max, min},
            {min, max, min, max},
            {max, min, max, min}
    };

    for (auto test : simple_tests) {
        auto ref_test = test;
        std::sort(ref_test.begin(), ref_test.end(), [](const size_t& left, const size_t& right) {return left < right;});
        Utils::radix_sort(test, [](const size_t& value) {return value;});

        REQUIRE(Equal(ref_test, test));
    }
}

TEST_CASE("Radix sort works") {
    Utils::FixedPointGenerator<size_t> generator;

    for (size_t test = 0; test < 100; ++test) {
        std::vector<TestStruct> values;
        std::vector<TestStruct> refValues;
        for (size_t i = 0; i < 1000; ++i) {
            values.push_back({i, generator.generateValue(std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::max())});
            refValues.push_back(values.back());
        }

        std::sort(refValues.begin(), refValues.end(), [](const TestStruct& left, const TestStruct& right) {return left.value < right.value;});
        Utils::radix_sort(values, [](const TestStruct& value) {return value.value;});

        REQUIRE(Equal(refValues, values));
    }

}
