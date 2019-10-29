#pragma once

#include <chrono>
#include <utility>

namespace Benchmark {
    template <class Function, class... Args>
    double measureExecutionTime(const Function& function, Args... args) {
        auto start = std::chrono::high_resolution_clock::now();
        function(std::forward<Args>(args)...);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;

        return elapsed.count();
    }
}
