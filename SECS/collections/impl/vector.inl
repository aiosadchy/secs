#ifndef SECS_VECTOR_INL
#define SECS_VECTOR_INL

#include <utility>
#include "SECS/collections/vector.hpp"

template <typename T>
Vector<T>::Vector(Index initial_capacity, double scale_factor) :
    m_reserved(0),
    m_size(0),
    m_data(nullptr),
    m_scale_factor(scale_factor) {
    if (m_scale_factor < 1.0) {
        m_scale_factor = 1.0;
    }
    reserve(initial_capacity);
}

template <typename T>
Vector<T>::Vector(const Vector &another) :
    m_reserved(0),
    m_size(0),
    m_data(nullptr),
    m_scale_factor(another.m_scale_factor) {
    reserve(another.m_reserved);
    for (const T &object : another) {
        emplace(object);
    }
}

template <typename T>
Vector<T>::Vector(Vector &&another) noexcept :
        m_reserved(another.m_reserved),
        m_size(another.m_size),
        m_data(another.m_data),
        m_scale_factor(another.m_scale_factor) {
    another.m_reserved = 0;
    another.m_size = 0;
    another.m_data = nullptr;
}

template <typename T>
Vector<T>::~Vector() {
    for (T &object : *this) {
        object.~T();
    }
    Memory::free<T>(m_data);
}

template <typename T>
template <typename ...Args>
T &Vector<T>::append(Args&& ...args) {
    if (m_size == m_reserved) {
        reserve(static_cast<Index>(m_reserved * m_scale_factor) + 1);
    }
    new (m_data + m_size) T(std::forward<Args>(args)...);
    return m_data[m_size++];
}

template <typename T>
void Vector<T>::pop() {
    m_data[--m_size].~T();
}

template <typename T>
T &Vector<T>::at(Index index) {
    return m_data[index];
}

template <typename T>
const T &Vector<T>::at(Index index) const {
    return const_cast<Vector<T> *>(this)->at(index);
}

template <typename T>
Index Vector<T>::size() const {
    return m_size;
}

template <typename T>
void Vector<T>::reserve(Index count) {
    if (count <= m_reserved) {
        return;
    }
    T *old_data = m_data;
    m_data = Memory::allocate<T>(count);
    for (Index i : Range(m_size)) {
        new (m_data + i) T(std::move(old_data[i]));
        old_data[i].~T();
    }
    m_reserved = count;
    Memory::free<T>(old_data);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin() {
    return m_data;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end() {
    return m_data + m_size;
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::begin() const {
    return const_cast<Vector<T> *>(this)->begin();
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::end() const {
    return const_cast<Vector<T> *>(this)->end();
}

#endif // SECS_VECTOR_INL
