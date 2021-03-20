#ifndef SECS_COMPONENT_POOL_INL
#define SECS_COMPONENT_POOL_INL

#include "SECS/collections/component_pool.hpp"

#include "SECS/collections/entity_pool.hpp"


namespace secs {

template <typename T>
template <typename Iterator>
ComponentPool<T>::GenericIterator<Iterator>::GenericIterator()
    : m_iterator() {
}

template <typename T>
template <typename Iterator>
ComponentPool<T>::GenericIterator<Iterator>::GenericIterator(const Iterator &iterator)
    : m_iterator(iterator) {
}

template <typename T>
template <typename Iterator>
typename ComponentPool<T>::template GenericIterator<Iterator> &
ComponentPool<T>::GenericIterator<Iterator>::operator++() {
    ++m_iterator;
    return *this;
}

template <typename T>
template <typename Iterator>
Entity ComponentPool<T>::GenericIterator<Iterator>::operator*() {
    return *m_iterator;
}

template <typename T>
template <typename Iterator>
bool ComponentPool<T>::GenericIterator<Iterator>::operator!=(const GenericIterator &another) const {
    return m_iterator != another.m_iterator;
}


template <typename T>
ComponentPool<T>::ComponentPool(Index initial_capacity)
    : m_data(initial_capacity, EntityPool::get_index) {
}

template <typename T>
typename ComponentPool<T>::Iterator ComponentPool<T>::begin() {
    return Iterator(m_data.keys().begin());
}

template <typename T>
typename ComponentPool<T>::End ComponentPool<T>::end() {
    return Iterator(m_data.keys().end());
}

template <typename T>
typename ComponentPool<T>::ConstIterator ComponentPool<T>::begin() const {
    return ConstIterator(m_data.keys().begin());
}

template <typename T>
typename ComponentPool<T>::ConstEnd ComponentPool<T>::end() const {
    return ConstIterator(m_data.keys().end());
}

template <typename T>
Index ComponentPool<T>::size() const {
    return m_data.size();
}

template <typename T>
template <typename... Args>
T &ComponentPool<T>::put(Entity entity, Args &&... args) {
    return m_data.put(entity, std::forward<Args>(args)...);
}

template <typename T>
void ComponentPool<T>::remove(Entity entity) {
    m_data.remove(entity);
}

template <typename T>
bool ComponentPool<T>::contains(Entity entity) const {
    return m_data.contains(entity);
}

template <typename T>
T &ComponentPool<T>::get(Entity entity) {
    return m_data.get(entity);
}

template <typename T>
const T &ComponentPool<T>::get(Entity entity) const {
    return m_data.get(entity);
}

template <typename T>
T *ComponentPool<T>::find(Entity entity) {
    return m_data.find(entity);
}

template <typename T>
const T *ComponentPool<T>::find(Entity entity) const {
    return m_data.find(entity);
}

} // namespace secs

#endif // SECS_COMPONENT_POOL_INL
