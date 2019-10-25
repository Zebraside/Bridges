#pragma once

#include <vector>
#include <assert.h>

namespace Utils {



template <class Container>
Container count_sort(const Container& container, size_t max) {
    return count_sort(container, max, [](auto val) {return val;});
}

template <class Container, class Func>
void count_sort(Container& container, size_t max, Func modifier) {
    std::vector<size_t> counter(max + 1);

    for (const auto& value : container) {
        ++counter[modifier(value)];
    }

    for (size_t i = 1; i < counter.size(); ++i) {
        counter[i] += counter[i - 1];
    }

    Container result(container.size());
    for (int i = container.size() - 1; i >= 0; --i) {
        auto value_idx = modifier(container[i]);
        result[counter[value_idx] - 1] = container[i];
        --counter[value_idx];
    }

    std::swap(container, result);
}

template <class Container>
void radix_sort(Container& container) {
    constexpr short bit_shift = 16;
    short shift_value = 0;
    size_t mask = 15;

    auto comparator = [&mask, &shift_value](auto value) {
        return (value & mask) >> shift_value;
    };

    size_t max_value = (~(0llu) & mask);
    for (size_t i = 0; i < sizeof(decltype(container[0])) / (bit_shift / 8); ++i) {
        count_sort(container, max_value, comparator);
        mask <<= bit_shift;
        shift_value += bit_shift;
    }
}

} // Utils
