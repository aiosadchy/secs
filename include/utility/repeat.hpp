//
// Created by aiosadchy on 28/09/2019.
//

#ifndef SECS_REPEAT_HPP
#define SECS_REPEAT_HPP

#include "range.hpp"

#define _SECS_REPEAT_CONCATENATE(x, y) x ## y

#define _SECS_REPEAT_COUNTER_NAME(line) _SECS_REPEAT_CONCATENATE(repeat_counter, line)

#define REPEAT(count) for ([[maybe_unused]] int _SECS_REPEAT_COUNTER_NAME(__LINE__) : Range(count))

#endif // SECS_REPEAT_HPP
