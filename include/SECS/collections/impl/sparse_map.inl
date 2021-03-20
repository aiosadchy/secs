#ifndef SECS_SPARSE_MAP_INL
#define SECS_SPARSE_MAP_INL

#include "SECS/collections/sparse_map.hpp"

#include <utility>


template <typename K, typename V, typename R>
SparseMap<K, V, R>::SparseMap(Index initial_capacity, R key_reduce)
    : m_dense()
    , m_sparse()
    , m_values()
    , m_key_reduce(key_reduce) {
    if (initial_capacity > 0) {
        m_dense.reserve(initial_capacity);
        m_sparse.reserve(initial_capacity);
        m_values.reserve(initial_capacity);
    }
}

template <typename K, typename V, typename R>
SparseMap<K, V, R>::~SparseMap() = default;

template <typename K, typename V, typename R>
template <typename ...Args>
V &SparseMap<K, V, R>::put(const K &key, Args&& ...args) {
    remove(key);
    Index key_index = m_key_reduce(key);
    if (m_sparse.size() <= key_index) {
        m_sparse.resize(key_index + 1, NULL_VALUE);
    }
    m_sparse[key_index] = static_cast<Index>(m_dense.size());
    m_dense.emplace_back(key);
    return m_values.emplace_back(std::forward<Args>(args)...);
}

template <typename K, typename V, typename R>
void SparseMap<K, V, R>::remove(const K &key) {
    if (!contains(key)) {
        return;
    }

    Index sparse_index = m_key_reduce(key);
    Index dense_index = m_sparse[sparse_index];

    m_sparse[sparse_index] = NULL_VALUE;

    if (dense_index != (m_dense.size() - 1)) {
        m_sparse[m_key_reduce(m_dense.back())] = dense_index;
        std::swap(m_values[dense_index], m_values.back());
        std::swap(m_dense[dense_index], m_dense.back());
    }

    m_values.pop_back();
    m_dense.pop_back();

}

template <typename K, typename V, typename R>
bool SparseMap<K, V, R>::contains(const K &key) const {
    Index index = m_key_reduce(key);
    return (m_sparse.size() > index) && (m_sparse[index] != NULL_VALUE);
}

template <typename K, typename V, typename R>
V &SparseMap<K, V, R>::get(const K &key) {
    return *find(key);
}

template <typename K, typename V, typename R>
const V &SparseMap<K, V, R>::get(const K &key) const {
    return *find(key);
}

template <typename K, typename V, typename R>
V *SparseMap<K, V, R>::find(const K &key) {
    return contains(key) ? &m_values[m_sparse[m_key_reduce(key)]] : nullptr;
}

template <typename K, typename V, typename R>
const V *SparseMap<K, V, R>::find(const K &key) const {
    return contains(key) ? &m_values[m_sparse[m_key_reduce(key)]] : nullptr;
}

template <typename K, typename V, typename R>
Index SparseMap<K, V, R>::size() const {
    return static_cast<Index>(m_dense.size());
}

template <typename K, typename V, typename R>
View<const SparseMap<K, V, R>, K> SparseMap<K, V, R>::keys() const {
    return View<const SparseMap<K, V, R>, K>(m_dense);
}

template <typename K, typename V, typename R>
View<SparseMap<K, V, R>, V> SparseMap<K, V, R>::values() {
    return View<SparseMap<K, V, R>, V>(m_values);
}

template <typename K, typename V, typename R>
View<const SparseMap<K, V, R>, V> SparseMap<K, V, R>::values() const {
    return View<const SparseMap<K, V, R>, V>(m_values);
}

#endif // SECS_SPARSE_MAP_INL
