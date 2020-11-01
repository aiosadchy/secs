#ifndef SECS_MEMORY_INL
#define SECS_MEMORY_INL

#include "SECS/memory.hpp"

template <typename T>
void Memory::free(T *memory) {
    delete [] reinterpret_cast<Storage<T> *>(memory);
}

template <typename T>
T *Memory::allocate(Index count) {
    return reinterpret_cast<T *>(new Storage<T> [count]);
}

#endif // SECS_MEMORY_INL
