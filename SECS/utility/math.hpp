#ifndef SECS_MATH_HPP
#define SECS_MATH_HPP

template <typename T>
inline T next_power_of_two(T x);

template <typename T>
inline T min(T first, T second);

template <typename T>
inline T max(T first, T second);

#endif // SECS_MATH_HPP

#ifndef SECS_MATH_INL
#include "impl/math.inl"
#endif
