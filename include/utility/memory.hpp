//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_MEMORY_HPP
#define SECS_MEMORY_HPP

#include "types.hpp"

class memory {
public:
    template <typename T>
    static T *allocate(Size count);

    template <typename T>
    static void free(T *memory);

private:
    template <typename T>
    class Placeholder;

};

#endif // SECS_MEMORY_HPP

#include "impl/memory.inl"
