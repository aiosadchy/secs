#ifndef SECS_SPARSE_MAP_VIEW_INL
#define SECS_SPARSE_MAP_VIEW_INL

#include "SECS/collections/sparse_map_view.hpp"


namespace secs {

namespace detail {

template <typename M, typename T>
SparseMapView<M, T>::Iterator::Iterator()
    : m_index(Index(0) - Index(1))
    , m_collection(nullptr) {
}

template <typename M, typename T>
SparseMapView<M, T>::Iterator::Iterator(Vector &collection, Index index)
    : m_index(index)
    , m_collection(&collection) {
}

template <typename M, typename T>
typename SparseMapView<M, T>::Iterator &SparseMapView<M, T>::Iterator::operator++() {
    m_index -= Index(1);
    return *this;
}

template <typename M, typename T>
typename SparseMapView<M, T>::Value &SparseMapView<M, T>::Iterator::operator*() {
    return (*m_collection)[m_index];
}

template <typename M, typename T>
bool SparseMapView<M, T>::Iterator::operator!=(const Iterator &another) const {
    return (m_index != another.m_index) || (m_collection != another.m_collection);
}


template <typename M, typename T>
SparseMapView<M, T>::SparseMapView(Vector &collection)
    : m_collection(&collection) {
}

template <typename M, typename T>
typename SparseMapView<M, T>::Iterator SparseMapView<M, T>::begin() {
    return Iterator(*m_collection, static_cast<Index>(m_collection->size()) - Index(1));
}

template <typename M, typename T>
typename SparseMapView<M, T>::Iterator SparseMapView<M, T>::end() {
    return Iterator(*m_collection, Index(0) - Index(1));
}

} // namespace detail

} // namespace secs

#endif // SECS_SPARSE_MAP_VIEW_INL
