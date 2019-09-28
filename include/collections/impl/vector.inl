//
// Created by aiosadchy on 28/09/2019.
//

#ifndef SECS_VECTOR_INL
#define SECS_VECTOR_INL

#include <new>
#include <utility>
#include "../vector.hpp"

template <typename T>
Vector<T>::Vector(Size initial_capacity) :
    m_reserved(0),
    m_size(0),
    m_data(nullptr) {
    reserve(initial_capacity);
}

template <typename T>
Vector<T>::~Vector() {
    for (T &object : *this) {
        object.~T();
    }
    free(m_data);
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
    return emplace(element);
}

template <typename T>
void Vector<T>::pop() {
    T *data = m_size > 0 ? m_data : nullptr;
    data[--m_size].~T();
}

template <typename T>
inline T &Vector<T>::operator[](Size index) {
    T *data = index < m_size ? m_data : nullptr;
    return data[index];
}

template <typename T>
inline const T &Vector<T>::operator[](Size index) const {
    return const_cast<Vector<T> *>(this)->operator[](index);
}

template <typename T>
Size Vector<T>::get_size() const {
    return m_size;
}

template <typename T>
T *Vector<T>::begin() {
    return m_data;
}

template <typename T>
T *Vector<T>::end() {
    return m_data + get_size();
}

template <typename T>
const T *Vector<T>::begin() const {
    return const_cast<Vector<T> *>(this)->begin();
}

template <typename T>
const T *Vector<T>::end() const {
    return const_cast<Vector<T> *>(this)->end();
}

template <typename T>
void Vector<T>::reserve(Size new_size) {
    T *old_data = m_data;
    void *new_data = ::operator new (sizeof(T) * new_size, std::align_val_t(alignof(T)));
    m_data = static_cast<T *>(new_data);
    for (Size i : Range<Size>(m_size)) {
        new (m_data + i) T(std::move(old_data[i]));
        old_data[i].~T();
    }
    m_reserved = new_size;
    free(old_data);
}

template <typename T>
void Vector<T>::free(T *data) {
    ::operator delete(data, std::align_val_t(alignof(T)));
}

#endif // SECS_VECTOR_INL
