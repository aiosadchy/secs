//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_MEMORY_INL
#define SECS_MEMORY_INL

#include "../memory.hpp"

template <typename T>
T *memory::allocate(Size count) {
    static_assert(alignof(Placeholder<T>) == alignof(T));
    static_assert(sizeof(Placeholder<T>) == sizeof(T));
    return reinterpret_cast<T *>(new Placeholder<T> [count]);
}

template <typename T>
void memory::free(T *memory) {
    static_assert(alignof(Placeholder<T>) == alignof(T));
    static_assert(sizeof(Placeholder<T>) == sizeof(T));
    delete [] reinterpret_cast<Placeholder<T> *>(memory);
}

template <typename T>
class memory::Placeholder {
public:
    Placeholder() = default;
    ~Placeholder() = default;

private:
    alignas(T) unsigned char m_data[sizeof(T)];

};

#endif // SECS_MEMORY_INL
