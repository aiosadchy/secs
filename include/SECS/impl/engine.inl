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
    , m_entity_pool(DEFAULT_INITIAL_CAPACITY) {
    m_component_pools.resize(Components::Metadata::get_registered_types_count());
    for (const auto &metadata : Components::iterate()) {
        Index type_index = metadata.get_type_id().get_index();
        m_component_pools[type_index].reset(metadata.create_pool(DEFAULT_INITIAL_CAPACITY));
    }
}

template <typename Family>
Engine<Family>::~Engine() {
    // TODO: implement entity iteration
    // for (Entity::ID entity : m_entity_pool) {
    //     destroy(entity);
    // }
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
template <typename Component, typename... Args>
auto &Engine<Family>::assign(const Entity::ID &entity, Args &&... args) {
    return get_component_pool<Component>().put(entity, std::forward<Args>(args)...);
}

template <typename Family>
template <typename... Component>
bool Engine<Family>::has(const Entity::ID &entity) const {
    return (get_component_pool<Component>().contains(entity) && ...);
}

template <typename Family>
template <typename... Component>
decltype(auto) Engine<Family>::get(const Entity::ID &entity) {
    if constexpr (sizeof...(Component) == 1) {
        return get_component_pool<Component...>().get(entity);
    } else {
        return std::forward_as_tuple(get<Component>(entity)...);
    }
}

template <typename Family>
template <typename... Component>
decltype(auto) Engine<Family>::get(const Entity::ID &entity) const {
    if constexpr (sizeof...(Component) == 1) {
        return get_component_pool<Component...>().get(entity);
    } else {
        return std::forward_as_tuple(get<Component>(entity)...);
    }
}

template <typename Family>
template <typename... Component>
decltype(auto) Engine<Family>::find(const Entity::ID &entity) {
    if constexpr (sizeof...(Component) == 1) {
        return get_component_pool<Component...>().find(entity);
    } else {
        return std::forward_as_tuple(find<Component>(entity)...);
    }
}

template <typename Family>
template <typename... Component>
decltype(auto) Engine<Family>::find(const Entity::ID &entity) const {
    if constexpr (sizeof...(Component) == 1) {
        return get_component_pool<Component...>().find(entity);
    } else {
        return std::forward_as_tuple(find<Component>(entity)...);
    }
}

template <typename Family>
template <typename... Component>
void Engine<Family>::remove(const Entity::ID &entity) {
    (get_component_pool<Component>().remove(entity), ...);
}

template <typename Family>
template <typename... Component>
typename Engine<Family>::template View<Component...> Engine<Family>::iterate() {
    return View<Component...>(*this);
}

template <typename Family>
template <typename... Component>
typename Engine<Family>::template ConstView<Component...> Engine<Family>::iterate() const {
    return ConstView<Component...>(*this);
}

template <typename Family>
View<EntityPool> Engine<Family>::iterate() {
    return ::View<EntityPool>(m_entity_pool);
}

template <typename Family>
View<const EntityPool> Engine<Family>::iterate() const {
    return ::View<const EntityPool>(m_entity_pool);
}

template <typename Family>
template <typename T>
typename Engine<Family>::Components::template Pool<T> &Engine<Family>::get_component_pool() {
    Index type_index = Components::TypeID::template get<T>().get_index();
    typename Components::AbstractPool *pool = m_component_pools[type_index].get();
    return fast_dynamic_cast<typename Components::template Pool<T> &>(*pool);
}

template <typename Family>
template <typename T>
const typename Engine<Family>::Components::template Pool<T> &Engine<Family>::get_component_pool() const {
    return const_cast<Engine<Family> *>(this)->get_component_pool<T>();
}

#endif // SECS_ENGINE_INL
