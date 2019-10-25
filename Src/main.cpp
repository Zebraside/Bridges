#include <iostream>

#include <algorithm>
#include <sort.hpp>
#include "graph.hpp"
#include "BridgeFinder.hpp"



int main() {
    std::vector<size_t> vec = {5, 3, 2, 1, 2, 4, 5};

    Utils::radix_sort(vec);

for (const auto& value : vec) {
        std::cout << value << " ";
    }

    std::vector<std::vector<size_t>> list = {
            {1, 2},
            {0},
            {0}
    };


    return 0;
}
