#ifndef SECS_COMPONENT_POOL_INL
#define SECS_COMPONENT_POOL_INL

#include "SECS/collections/component_pool.hpp"


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
Entity::ID ComponentPool<T>::GenericIterator<Iterator>::operator*() {
    // TODO: fix hardcoded entity version
    return Entity::ID(*m_iterator, 0);
}

template <typename T>
template <typename Iterator>
template <typename Another>
bool ComponentPool<T>::GenericIterator<Iterator>::operator!=(const Another &another) const {
    return m_iterator != another.m_iterator;
}


template <typename T>
ComponentPool<T>::ComponentPool(Index initial_capacity)
    : m_data(initial_capacity) {
}

template <typename T>
typename ComponentPool<T>::Iterator ComponentPool<T>::begin() {
    return Iterator(m_data.keys().begin());
}

template <typename T>
typename ComponentPool<T>::Iterator ComponentPool<T>::end() {
    return Iterator(m_data.keys().end());
}

template <typename T>
typename ComponentPool<T>::ConstIterator ComponentPool<T>::begin() const {
    return ConstIterator(m_data.keys().begin());
}

template <typename T>
typename ComponentPool<T>::ConstIterator ComponentPool<T>::end() const {
    return ConstIterator(m_data.keys().end());
}

template <typename T>
Index ComponentPool<T>::size() const {
    return m_data.size();
}

template <typename T>
template <typename... Args>
T &ComponentPool<T>::put(Entity::ID key, Args &&... args) {
    return m_data.put(key.get_index(), std::forward<Args>(args)...);
}

template <typename T>
void ComponentPool<T>::remove(Entity::ID key) {
    m_data.remove(key.get_index());
}

template <typename T>
bool ComponentPool<T>::contains(Entity::ID key) const {
    return m_data.contains(key.get_index());
}

template <typename T>
T &ComponentPool<T>::get(Entity::ID key) {
    return m_data.get(key.get_index());
}

template <typename T>
const T &ComponentPool<T>::get(Entity::ID key) const {
    return m_data.get(key.get_index());
}

template <typename T>
T *ComponentPool<T>::find(Entity::ID key) {
    return m_data.find(key.get_index());
}

template <typename T>
const T *ComponentPool<T>::find(Entity::ID key) const {
    return m_data.find(key.get_index());
}

#endif // SECS_COMPONENT_POOL_INL
