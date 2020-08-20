#ifndef SECS_MATH_INL
#define SECS_MATH_INL

#include "SECS/utility/math.hpp"

template <typename T>
T min(T first, T second) {
    return (first < second) ? first : second;
}

template <typename T>
T max(T first, T second) {
    return (first < second) ? second : first;
}

#endif // SECS_MATH_INL
