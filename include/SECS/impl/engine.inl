#ifndef SECS_ENGINE_INL
#define SECS_ENGINE_INL

#include "SECS/engine.hpp"

#include <tuple>
#include <utility>

#include "SECS/utility.hpp"


namespace secs {

// TODO: change to something customizable
constexpr Index DEFAULT_INITIAL_CAPACITY = 32;

template <typename Family>
Engine<Family>::Engine()
    : m_component_pools()
    , m_entity_pool(DEFAULT_INITIAL_CAPACITY, 16)
    , m_event_manager() {
    for (const auto &metadata : Components::view()) {
        Index type_index = metadata.get_type_id().get_index();
        if (type_index >= m_component_pools.size()) {
            m_component_pools.resize(type_index + 1);
        }
        m_component_pools[type_index] = std::move(metadata.create_pool(DEFAULT_INITIAL_CAPACITY));
    }
}

template <typename Family>
Engine<Family>::~Engine() {
    for (Entity entity : m_entity_pool) {
        destroy(entity);
    }
}

template <typename Family>
Entity Engine<Family>::create() {
    return m_entity_pool.create();
}

template <typename Family>
void Engine<Family>::destroy(Entity entity) {
    for (auto &pool : m_component_pools) {
        // TODO: raise EntityLostComponent event
        // TODO: optimize virtual call to IComponentPool::remove(Entity)
        pool->remove(entity);
    }
    m_entity_pool.destroy(entity);
}

template <typename Family>
bool Engine<Family>::is_alive(Entity entity) const {
    return m_entity_pool.is_alive(entity);
}

template <typename Family>
template <typename C, typename... Args>
auto &Engine<Family>::assign(Entity entity, Args &&... args) {
    return get_component_pool<C>().put(entity, std::forward<Args>(args)...);
}

template <typename Family>
template <typename... C>
bool Engine<Family>::has(Entity entity) const {
    return (get_component_pool<C>().contains(entity) && ...);
}

template <typename Family>
template <typename... C>
decltype(auto) Engine<Family>::get(Entity entity) {
    if constexpr (sizeof...(C) == 1) {
        return get_component_pool<C...>().get(entity);
    } else {
        return std::forward_as_tuple(get<C>(entity)...);
    }
}

template <typename Family>
template <typename... C>
decltype(auto) Engine<Family>::get(Entity entity) const {
    if constexpr (sizeof...(C) == 1) {
        return get_component_pool<C...>().get(entity);
    } else {
        return std::forward_as_tuple(get<C>(entity)...);
    }
}

template <typename Family>
template <typename... C>
decltype(auto) Engine<Family>::find(Entity entity) {
    if constexpr (sizeof...(C) == 1) {
        return get_component_pool<C...>().find(entity);
    } else {
        return std::forward_as_tuple(find<C>(entity)...);
    }
}

template <typename Family>
template <typename... C>
decltype(auto) Engine<Family>::find(Entity entity) const {
    if constexpr (sizeof...(C) == 1) {
        return get_component_pool<C...>().find(entity);
    } else {
        return std::forward_as_tuple(find<C>(entity)...);
    }
}

template <typename Family>
template <typename... C>
void Engine<Family>::remove(Entity entity) {
    if constexpr (sizeof...(C) == 1) {
        if (has<C...>(entity)) {
            using EventType = typename Events::template EntityLostComponent<C...>;
            typename Components::template Pool<C...> &pool = get_component_pool<C...>();
            m_event_manager.template handle<EventType>(entity, pool.get(entity));
            pool.remove(entity);
        }
    } else {
        (remove<C>(entity), ...);
    }
}

template <typename Family>
template <typename... C>
View<Engine<Family>, C...> Engine<Family>::view() {
    return View<Engine<Family>, C...>(*this, get_component_pool<C>()...);
}

template <typename Family>
template <typename... C>
View<const Engine<Family>, C...> Engine<Family>::view() const {
    return View<const Engine<Family>, C...>(*this, get_component_pool<C>()...);
}

template <typename Family>
View<EntityPool> Engine<Family>::view() {
    return View<EntityPool>(m_entity_pool);
}

template <typename Family>
View<const EntityPool> Engine<Family>::view() const {
    return View<const EntityPool>(m_entity_pool);
}

template <typename Family>
EventManager<Family> &Engine<Family>::get_event_manager() {
    return m_event_manager;
}

template <typename Family>
template <typename T>
typename Engine<Family>::Components::template Pool<T> &Engine<Family>::get_component_pool() {
    Index type_index = Components::Metadata::template get<T>().get_type_id().get_index();
    typename Components::IPool *pool = m_component_pools[type_index].get();
    return fast_dynamic_cast<typename Components::template Pool<T> &>(*pool);
}

template <typename Family>
template <typename T>
const typename Engine<Family>::Components::template Pool<T> &Engine<Family>::get_component_pool() const {
    Index type_index = Components::Metadata::template get<T>().get_type_id().get_index();
    typename Components::IPool *pool = m_component_pools[type_index].get();
    return fast_dynamic_cast<typename Components::template Pool<T> &>(*pool);
}

} // namespace secs

#endif // SECS_ENGINE_INL
