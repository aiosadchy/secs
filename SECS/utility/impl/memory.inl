#ifndef SECS_MEMORY_INL
#define SECS_MEMORY_INL

#include "SECS/utility/memory.hpp"

template <typename T>
T *Memory::allocate(Size count) {
    return reinterpret_cast<T *>(new Placeholder<T> [count]);
}

template <typename T>
void Memory::free(T *memory) {
    delete [] reinterpret_cast<Placeholder<T> *>(memory);
}

template <typename T>
Memory::Placeholder<T>::Placeholder() :
    m_data() {
    static_assert(alignof(Placeholder<T>) == alignof(T));
    static_assert(sizeof(Placeholder<T>) == sizeof(T));
}

template <typename T>
T *Memory::Placeholder<T>::as_object() {
    return reinterpret_cast<T *>(m_data);
}

template <typename T>
const T *Memory::Placeholder<T>::as_object() const {
    return const_cast<Placeholder<T> *>(this)->as_object();
}

#endif // SECS_MEMORY_INL
