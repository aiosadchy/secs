//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_SPARSE_MAP_INL
#define SECS_SPARSE_MAP_INL

#include "../sparse_map.hpp"

template <typename T>
SparseMap<T>::SparseMap(Size initial_capacity) :
    m_dense(initial_capacity),
    m_sparse(initial_capacity),
    m_values(initial_capacity) {}

template <typename T>
SparseMap<T>::~SparseMap() = default;

template <typename T>
template <typename... Args>
T &SparseMap<T>::put(Size key, Args&& ...args) {
    if (m_sparse.get_size() <= key) {
        Size old_size = m_sparse.get_size();
        Size multiplier = (old_size > 0) ? (key / old_size) : key;
        Size new_size = next_power_of_two(multiplier) * max(old_size, Size(1));
        m_sparse.reserve(new_size);
        REPEAT(new_size - old_size) {
            m_sparse.append(0);
        }
    }
    remove(key);
    m_dense.append(key);
    m_sparse[key] = m_dense.get_size() - 1;
    return m_values.emplace(std::forward<Args>(args)...);
}

template <typename T>
void SparseMap<T>::remove(Size key) {
    if (!contains(key)) {
        return;
    }

    Size index = m_sparse[key];

    m_values[index].~T();
    new (&m_values[index]) T(std::move(m_values[m_values.get_size() - 1]));
    m_values.pop();

    Size last = m_dense[m_dense.get_size() - 1];
    m_sparse[last] = index;
    m_dense[index] = last;
    m_dense.pop();
}

template <typename T>
bool SparseMap<T>::contains(Size key) const {
    return (m_sparse.get_size() > key)
        && (m_dense.get_size() > m_sparse[key])
        && (m_dense[m_sparse[key]] == key);
}

template <typename T>
T &SparseMap<T>::get(Size key) {
    T *object = contains(key) ? &m_values[m_sparse[key]] : nullptr;
    return *object;
}

template <typename T>
const T &SparseMap<T>::get(Size key) const {
    return const_cast<SparseMap<T> *>(this)->get(key);
}

template <typename T>
T &SparseMap<T>::operator[](Size key) {
    return contains(key) ? m_values[m_sparse[key]] : put(key);
}

template <typename T>
const T &SparseMap<T>::operator[](Size key) const {
    return const_cast<SparseMap<T> *>(this)->operator[](key);
}

template <typename T>
Size SparseMap<T>::get_size() const {
    return m_dense.get_size();
}

template <typename T>
View<const Vector<Size>> SparseMap<T>::get_keys() const {
    return View<const Vector<Size>>(m_dense);
}

template <typename T>
View<Vector<T>> SparseMap<T>::get_values() {
    return View<Vector<T>>(m_values);
}

template <typename T>
View<const Vector<T>> SparseMap<T>::get_values() const {
    return View<const Vector<T>>(m_values);
}

#endif // SECS_SPARSE_MAP_INL
