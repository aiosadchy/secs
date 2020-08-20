#ifndef SECS_MEMORY_INL
#define SECS_MEMORY_INL

#include "SECS/utility/memory.hpp"

template <typename T>
T *Memory::allocate(Index count) {
    static_assert(alignof(Storage<T>) == alignof(T));
    static_assert(sizeof(Storage<T>) == sizeof(T));
    return reinterpret_cast<T *>(new Storage<T> [count]);
}

template <typename T>
void Memory::free(T *memory) {
    delete [] reinterpret_cast<Storage<T> *>(memory);
}

template <typename T>
T *Memory::Storage<T>::ptr() {
    return reinterpret_cast<T *>(m_data);
}

template <typename T>
const T *Memory::Storage<T>::ptr() const {
    return const_cast<Storage <T> *>(this)->ptr();
}

template <typename T>
T &Memory::Storage<T>::ref() {
    return *ptr();
}

template <typename T>
const T &Memory::Storage<T>::ref() const {
    return const_cast<Storage <T> *>(this)->ref();
}

#endif // SECS_MEMORY_INL
