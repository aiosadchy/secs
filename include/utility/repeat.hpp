//
// Created by aiosadchy on 28/09/2019.
//

#ifndef SECS_REPEAT_HPP
#define SECS_REPEAT_HPP

#include "range.hpp"

#define _REPEAT_IMPL_CONCATENATE(x, y) x ## y

#define _REPEAT_IMPL_COUNTER_NAME(line) _REPEAT_IMPL_CONCATENATE(repeat_counter, line)

#define REPEAT(count) for ([[maybe_unused]] int _REPEAT_IMPL_COUNTER_NAME(__LINE__) : Range(count))

#endif // SECS_REPEAT_HPP
