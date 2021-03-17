#ifndef SECS_ENGINE_INL
#define SECS_ENGINE_INL

#include "SECS/engine.hpp"

#include "SECS/utility.hpp"

#include <tuple>

// TODO: change to something customizable
constexpr Index DEFAULT_INITIAL_CAPACITY = 32;

template <typename Family>
Engine<Family>::Engine()
    : m_component_pools()
    , m_entity_pool(DEFAULT_INITIAL_CAPACITY, 16) {
    for (const auto &metadata : Components::iterate()) {
        Index type_index = metadata.get_type_id().get_index();
        if (type_index >= m_component_pools.size()) {
            m_component_pools.resize(type_index + 1);
        }
        m_component_pools[type_index].reset(metadata.create_pool(DEFAULT_INITIAL_CAPACITY));
    }
}

template <typename Family>
Engine<Family>::~Engine() {
    for (Entity::ID entity : m_entity_pool) {
        destroy(entity);
    }
}

template <typename Family>
Entity::ID Engine<Family>::create() {
    return m_entity_pool.create();
}

template <typename Family>
void Engine<Family>::destroy(const Entity::ID &entity) {
    for (auto &pool : m_component_pools) {
        pool->remove(entity);
    }
    m_entity_pool.destroy(entity);
}

template <typename Family>
bool Engine<Family>::is_alive(const Entity::ID &entity) const {
    return m_entity_pool.is_alive(entity);
}

template <typename Family>
template <typename C, typename... Args>
auto &Engine<Family>::assign(const Entity::ID &entity, Args &&... args) {
    return get_component_pool<C>().put(entity, std::forward<Args>(args)...);
}

template <typename Family>
template <typename... C>
bool Engine<Family>::has(const Entity::ID &entity) const {
    return (get_component_pool<C>().contains(entity) && ...);
}

template <typename Family>
template <typename... C>
decltype(auto) Engine<Family>::get(const Entity::ID &entity) {
    if constexpr (sizeof...(C) == 1) {
        return get_component_pool<C...>().get(entity);
    } else {
        return std::forward_as_tuple(get<C>(entity)...);
    }
}

template <typename Family>
template <typename... C>
decltype(auto) Engine<Family>::get(const Entity::ID &entity) const {
    if constexpr (sizeof...(C) == 1) {
        return get_component_pool<C...>().get(entity);
    } else {
        return std::forward_as_tuple(get<C>(entity)...);
    }
}

template <typename Family>
template <typename... C>
decltype(auto) Engine<Family>::find(const Entity::ID &entity) {
    if constexpr (sizeof...(C) == 1) {
        return get_component_pool<C...>().find(entity);
    } else {
        return std::forward_as_tuple(find<C>(entity)...);
    }
}

template <typename Family>
template <typename... C>
decltype(auto) Engine<Family>::find(const Entity::ID &entity) const {
    if constexpr (sizeof...(C) == 1) {
        return get_component_pool<C...>().find(entity);
    } else {
        return std::forward_as_tuple(find<C>(entity)...);
    }
}

template <typename Family>
template <typename... C>
void Engine<Family>::remove(const Entity::ID &entity) {
    (get_component_pool<C>().remove(entity), ...);
}

template <typename Family>
template <typename... C>
View<Engine<Family>, C...> Engine<Family>::iterate() {
    return View<Engine<Family>, C...>(*this, get_component_pool<C>()...);
}

template <typename Family>
template <typename... C>
View<const Engine<Family>, C...> Engine<Family>::iterate() const {
    return View<const Engine<Family>, C...>(*this, get_component_pool<C>()...);
}

template <typename Family>
View<EntityPool> Engine<Family>::iterate() {
    return View<EntityPool>(m_entity_pool);
}

template <typename Family>
View<const EntityPool> Engine<Family>::iterate() const {
    return View<const EntityPool>(m_entity_pool);
}

template <typename Family>
template <typename T>
typename Engine<Family>::Components::template Pool<T> &Engine<Family>::get_component_pool() {
    Index type_index = Components::Metadata::template get<T>().get_type_id().get_index();
    typename Components::AbstractPool *pool = m_component_pools[type_index].get();
    return fast_dynamic_cast<typename Components::template Pool<T> &>(*pool);
}

template <typename Family>
template <typename T>
const typename Engine<Family>::Components::template Pool<T> &Engine<Family>::get_component_pool() const {
    Index type_index = Components::Metadata::template get<T>().get_type_id().get_index();
    typename Components::AbstractPool *pool = m_component_pools[type_index].get();
    return fast_dynamic_cast<typename Components::template Pool<T> &>(*pool);
}

#endif // SECS_ENGINE_INL
