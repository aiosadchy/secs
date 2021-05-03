#ifndef SECS_ENGINE_INL
#define SECS_ENGINE_INL

#include "SECS/engine.hpp"

#include <tuple>
#include <utility>

#include "SECS/utility.hpp"


namespace secs {

// TODO: change to something customizable
constexpr Index DEFAULT_INITIAL_CAPACITY = 32;
constexpr Index DEFAULT_RECYCLE_PERIOD = 16;

template <typename Family>
Engine<Family>::Engine()
    : m_component_pools()
    , m_remove_component()
    , m_entity_pool(DEFAULT_INITIAL_CAPACITY, DEFAULT_RECYCLE_PERIOD)
    , m_event_manager() {
    for (const auto &metadata : Metadata::view()) {
        register_component(metadata);
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
    Entity entity = m_entity_pool.create();
    m_event_manager.template handle<typename Events::EntityCreated>(entity);
    return entity;
}

template <typename Family>
void Engine<Family>::destroy(Entity entity) {
    for (const auto remove_component : m_remove_component) {
        remove_component(*this, entity);
    }
    m_event_manager.template handle<typename Events::EntityDestroyed>(entity, m_entity_pool);
    SECS_IF_DEBUG (
        debug::IteratorRegistry::on_destroy(entity);
    )
}

template <typename Family>
bool Engine<Family>::is_alive(Entity entity) const {
    return m_entity_pool.is_alive(entity);
}

template <typename Family>
template <typename C, typename... Args>
auto &Engine<Family>::assign(Entity entity, Args &&... args) {
    using Event = typename Events::template EntityGotComponent<C>;
    typename Components::template Pool<C> &pool = get_component_pool<C>();
    C &component = pool.put(entity, std::forward<Args>(args)...);
    m_event_manager.template handle<Event>(entity, pool);
    return component;
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
            using Event = typename Events::template EntityLostComponent<C...>;
            m_event_manager.template handle<Event>(entity, get_component_pool<C...>());
        }
        SECS_IF_DEBUG (
            debug::IteratorRegistry::on_remove<C...>(entity);
        )
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
template <typename... C>
void Engine<Family>::register_component() {
    (register_component(Metadata::template get<C>()), ...);
}


template <typename Family>
template <typename T>
Engine<Family>::Metadata::Metadata(typename Metadata::template Initializer<T>)
    : LinkedMetadata<Metadata, Decay>(this)
    , create_pool(
        +[](Index initial_capacity) {
            return PoolHandle(new typename Components::template Pool<T>(initial_capacity));
        })
    , remove_component(
        +[](Engine &engine, Entity entity) {
            engine.template remove<T>(entity);
        })
    , m_type_id(TypeID::template get<T>()) {
}

template <typename Family>
typename Engine<Family>::TypeID Engine<Family>::Metadata::get_type_id() const {
    return m_type_id;
}


template <typename Family>
template <typename T>
typename Engine<Family>::Components::template Pool<T> &Engine<Family>::get_component_pool() {
    Index type_index = Metadata::template get<T>().get_type_id().get_index();
    typename Components::IPool *pool = m_component_pools[type_index].get();
    return SECS_FAST_DYNAMIC_CAST<typename Components::template Pool<T> &>(*pool);
}

template <typename Family>
template <typename T>
const typename Engine<Family>::Components::template Pool<T> &Engine<Family>::get_component_pool() const {
    Index type_index = Metadata::template get<T>().get_type_id().get_index();
    typename Components::IPool *pool = m_component_pools[type_index].get();
    return SECS_FAST_DYNAMIC_CAST<typename Components::template Pool<T> &>(*pool);
}

template <typename Family>
void Engine<Family>::register_component(const Engine::Metadata &metadata) {
    Index type_index = metadata.get_type_id().get_index();
    if (type_index >= m_component_pools.size()) {
        m_component_pools.resize(type_index + 1);
    }
    if (type_index >= m_remove_component.size()) {
        m_remove_component.resize(type_index + 1);
    }
    m_component_pools.at(type_index) = std::move(metadata.create_pool(DEFAULT_INITIAL_CAPACITY));
    m_remove_component.at(type_index) = metadata.remove_component;
}

} // namespace secs

#endif // SECS_ENGINE_INL
