#ifndef FAST_FIXED_H
#define FAST_FIXED_H

#include <cstdint>
#include <type_traits>

template <int N, int K>
class FastFixed {
    using BaseType = typename std::conditional<N <= 32, int_fast32_t, int_fast64_t>::type;
    BaseType value;

public:
    FastFixed(double x = 0) : value(static_cast<BaseType>(x* (1 << K))) {}

    FastFixed operator+(const FastFixed& other) const {
        return FastFixed::fromRaw(value + other.value);
    }

    FastFixed operator-(const FastFixed& other) const {
        return FastFixed::fromRaw(value - other.value);
    }

    FastFixed operator*(const FastFixed& other) const {
        return FastFixed::fromRaw((value * other.value) >> K);
    }

    FastFixed operator/(const FastFixed& other) const {
        return FastFixed::fromRaw((value << K) / other.value);
    }

    double toDouble() const {
        return static_cast<double>(value) / (1 << K);
    }

    static FastFixed fromRaw(BaseType rawValue) {
        FastFixed f;
        f.value = rawValue;
        return f;
    }
};

#endif // FAST_FIXED_H
