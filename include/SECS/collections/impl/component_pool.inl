#ifndef SECS_COMPONENT_POOL_INL
#define SECS_COMPONENT_POOL_INL

#include "SECS/collections/component_pool.hpp"

template <typename T>
ComponentPool<T>::ComponentPool(Index initial_capacity)
    : m_data(initial_capacity) {
}

template <typename T>
auto ComponentPool<T>::begin() {
    return m_data.keys().begin();
}

template <typename T>
auto ComponentPool<T>::begin() const {
    return m_data.keys().begin();
}

template <typename T>
auto ComponentPool<T>::end() {
    return m_data.keys().end();
}

template <typename T>
auto ComponentPool<T>::end() const {
    return m_data.keys().end();
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
