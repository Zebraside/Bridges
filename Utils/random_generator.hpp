#pragma once

#include <limits>
#include <type_traits>
#include <random>

namespace Utils {

template <typename DataType>
class RandomGenerator {
public:
    virtual DataType generateValue(DataType min = std::numeric_limits<DataType>::min(), DataType max = std::numeric_limits<DataType>::max()) const = 0;
    virtual ~RandomGenerator() = default;
};

template <typename DataType>
class FixedPointGenerator : public RandomGenerator<DataType> {
public:
    FixedPointGenerator()
    : rng(dev())
    , dist(std::numeric_limits<DataType>::min(), std::numeric_limits<DataType>::max()) {
        static_assert(std::is_integral<DataType>::value);
    }

    DataType generateValue(DataType min, DataType max) const override {
        if (min == max) {
            return min;
        }

        return min + (dist(rng) % (max - min));
    }

private:
    std::random_device dev;
    mutable std::mt19937 rng;
    mutable std::uniform_int_distribution<DataType> dist;
};

template <typename DataType>
class FlaotingPointGenerator : public RandomGenerator<DataType> {
public:
    FlaotingPointGenerator()
            : rng(dev())
            , dist(std::numeric_limits<DataType>::min(), std::numeric_limits<DataType>::max()) {
        static_assert(std::is_floating_point<DataType>::value);
    }

    DataType generateValue(DataType min, DataType max) const override {
        if (min == max) {
            return min;
        }

        return min + (dist(rng) / (std::numeric_limits<DataType>::max() / max));
    }
private:
    std::random_device dev;
    mutable std::mt19937 rng;
    mutable std::uniform_real_distribution<DataType> dist;
};

} // Utils
