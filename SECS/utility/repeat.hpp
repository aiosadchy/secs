#ifndef SECS_REPEAT_HPP
#define SECS_REPEAT_HPP

#include "range.hpp"

#define _SECS_REPEAT_CONCATENATE(x, y) x ## y

#define _SECS_REPEAT_COUNTER_NAME(line) _SECS_REPEAT_CONCATENATE(_secs_repeat_counter, line)

#define REPEAT(count) for ([[maybe_unused]] auto _SECS_REPEAT_COUNTER_NAME(__LINE__) : Range(count))

#endif // SECS_REPEAT_HPP
