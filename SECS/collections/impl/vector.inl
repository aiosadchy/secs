//
// Created by aiosadchy on 28/09/2019.
//

#ifndef SECS_VECTOR_INL
#define SECS_VECTOR_INL

#include <utility>
#include "SECS/collections/vector.hpp"

template <typename T>
Vector<T>::Vector(Size initial_capacity) :
    m_reserved(0),
    m_size(0),
    m_data(nullptr) {
    reserve(initial_capacity);
}

template <typename T>
Vector<T>::Vector(const Vector &another) :
    m_reserved(0),
    m_size(0),
    m_data(nullptr) {
    reserve(another.m_reserved);
    for (const T &object : another) {
        emplace(object);
    }
}

template <typename T>
Vector<T>::Vector(Vector &&another) noexcept :
        m_reserved(another.m_reserved),
        m_size(another.m_size),
        m_data(another.m_data) {
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
template <typename... Args>
T &Vector<T>::emplace(Args&& ...args) {
    if (m_size == m_reserved) {
        reserve(m_reserved * 2);
    }
    new (m_data + m_size) T(std::forward<Args>(args)...);
    return m_data[m_size++];
}

template <typename T>
T &Vector<T>::append(const T &element) {
    return emplace(element);
}

template <typename T>
T &Vector<T>::append(T &&element) {
    return emplace(std::move(element));
}

template <typename T>
void Vector<T>::pop() {
    T *object = (m_size > 0) ? (m_data + m_size - 1) : nullptr;
    object->~T();
    --m_size;
}

template <typename T>
T &Vector<T>::operator[](Size index) {
    T *object = (index < m_size) ? (m_data + index) : nullptr;
    return *object;
}

template <typename T>
const T &Vector<T>::operator[](Size index) const {
    return const_cast<Vector<T> *>(this)->operator[](index);
}

template <typename T>
Size Vector<T>::get_size() const {
    return m_size;
}

template <typename T>
void Vector<T>::reserve(Size count) {
    if (count == 0) {
        count = 1;
    }
    if (count <= m_reserved) {
        return;
    }
    T *old_data = m_data;
    m_data = Memory::allocate<T>(count);
    for (Size i : Range<Size>(m_size)) {
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
    return m_data + get_size();
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
