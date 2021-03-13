#ifndef SECS_MEMORY_HPP
#define SECS_MEMORY_HPP

#include <utl/storage.hpp>
#include "SECS/common.hpp"

class Memory {
public:
    Memory() = delete;

    template <typename T>
    using Storage = utl::Storage<T>;

    template <typename T>
    static T *allocate(Index count);

    template <typename T>
    static void free(T *memory);

};

#endif // SECS_MEMORY_HPP
