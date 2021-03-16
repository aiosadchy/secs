#ifndef SECS_SPARSE_MAP_INL
#define SECS_SPARSE_MAP_INL

#include "SECS/collections/sparse_map.hpp"

#include <utility>


template <typename T>
SparseMap<T>::SparseMap(Index initial_capacity, double reserve_factor)
    : m_dense()
    , m_sparse()
    , m_values()
    , m_reserve_factor(reserve_factor) {
    if (initial_capacity > 0) {
        m_dense.reserve(initial_capacity);
        m_sparse.reserve(initial_capacity);
        m_values.reserve(initial_capacity);
    }
}

template <typename T>
SparseMap<T>::~SparseMap() = default;

template <typename T>
template <typename ...Args>
T &SparseMap<T>::put(Index key, Args&& ...args) {
    remove(key);
    if (m_sparse.size() <= key) {
        double scaling_factor = m_reserve_factor + 1.0;
        Index new_size = static_cast<Index>(key * scaling_factor) + 1;
        m_sparse.resize(new_size, NULL_VALUE);
    }
    m_sparse[key] = m_dense.size();
    m_dense.emplace_back(key);
    return m_values.emplace_back(std::forward<Args>(args)...);
}

template <typename T>
void SparseMap<T>::remove(Index key) {
    if (!contains(key)) {
        return;
    }

    Index index = m_sparse[key];
    m_sparse[key] = NULL_VALUE;

    std::swap(m_values[index], m_values.back());
    m_values.pop_back();

    Index last = m_dense.back();
    m_sparse[last] = index;
    m_dense[index] = last;
    m_dense.pop_back();
}

template <typename T>
bool SparseMap<T>::contains(Index key) const {
    return (m_sparse.size() > key) && (m_sparse[key] != NULL_VALUE);
}

template <typename T>
T &SparseMap<T>::get(Index key) {
    return *find(key);
}

template <typename T>
const T &SparseMap<T>::get(Index key) const {
    return *find(key);
}

template <typename T>
T *SparseMap<T>::find(Index key) {
    return contains(key) ? &m_values[m_sparse[key]] : nullptr;
}

template <typename T>
const T *SparseMap<T>::find(Index key) const {
    return contains(key) ? &m_values[m_sparse[key]] : nullptr;
}

template <typename T>
Index SparseMap<T>::size() const {
    return m_dense.size();
}

template <typename T>
View<const std::vector<Index>> SparseMap<T>::keys() const {
    return View<const std::vector<Index>>(m_dense);
}

template <typename T>
View<std::vector<T>> SparseMap<T>::values() {
    return View<std::vector<T>>(m_values);
}

template <typename T>
View<const std::vector<T>> SparseMap<T>::values() const {
    return View<const std::vector<T>>(m_values);
}

#endif // SECS_SPARSE_MAP_INL
