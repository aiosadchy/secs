#ifndef SECS_SPARSE_MAP_INL
#define SECS_SPARSE_MAP_INL

#include <utl/repeat.hpp>
#include "SECS/collections/sparse_map.hpp"

template <typename T>
SparseMap<T>::SparseMap(Index initial_capacity, double reserve_factor, double growth_rate) :
        m_dense(initial_capacity, growth_rate),
        m_sparse(initial_capacity, growth_rate),
        m_values(initial_capacity, growth_rate),
        m_reserve_factor(reserve_factor) {
    if (m_reserve_factor < 0.0) {
        m_reserve_factor = 0.0;
    }
}

template <typename T>
SparseMap<T>::~SparseMap() = default;

template <typename T>
template <typename ...Args>
T &SparseMap<T>::put(Index key, Args&& ...args) {
    remove(key);
    if (m_sparse.size() <= key) {
        Index old_size = m_sparse.size();
        double scaling_factor = m_reserve_factor + 1.0;
        Index new_size = static_cast<Index>(key * scaling_factor) + 1;
        m_sparse.reserve(new_size);
        UTL_REPEAT(new_size - old_size) {
            m_sparse.append(0);
        }
    }
    m_dense.append(key);
    m_sparse.at(key) = m_dense.size() - 1;
    return m_values.append(std::forward<Args>(args)...);
}

template <typename T>
void SparseMap<T>::remove(Index key) {
    if (!contains(key)) {
        return;
    }

    Index index = m_sparse.at(key);

    m_values.at(index).~T();
    new (&m_values.at(index)) T(std::move(m_values.at(m_values.size() - 1)));
    m_values.pop();

    Index last = m_dense.at(m_dense.size() - 1);
    m_sparse.at(last) = index;
    m_dense.at(index) = last;
    m_dense.pop();
}

template <typename T>
bool SparseMap<T>::contains(Index key) const {
    return (m_sparse.size() > key)
        && (m_dense.size() > m_sparse.at(key))
        && (m_dense.at(m_sparse.at(key)) == key);
}

template <typename T>
T &SparseMap<T>::get(Index key) {
    return *find(key);
}

template <typename T>
const T &SparseMap<T>::get(Index key) const {
    return const_cast<SparseMap<T> *>(this)->get(key);
}

template <typename T>
T *SparseMap<T>::find(Index key) {
    return contains(key) ? &m_values.at(m_sparse.at(key)) : nullptr;
}

template <typename T>
const T *SparseMap<T>::find(Index key) const {
    return const_cast<SparseMap<T> *>(this)->find(key);
}

template <typename T>
Index SparseMap<T>::size() const {
    return m_dense.size();
}

template <typename T>
View<const Vector<Index>> SparseMap<T>::keys() const {
    return View<const Vector<Index>>(m_dense);
}

template <typename T>
View<Vector<T>> SparseMap<T>::values() {
    return View<Vector<T>>(m_values);
}

template <typename T>
View<const Vector<T>> SparseMap<T>::values() const {
    return View<const Vector<T>>(m_values);
}

#endif // SECS_SPARSE_MAP_INL
