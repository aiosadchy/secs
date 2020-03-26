#ifndef SECS_ENTITY_MANAGER_INL
#define SECS_ENTITY_MANAGER_INL

#include "SECS/entity_manager.hpp"

#ifndef SECS_COMPONENT_POOL_DEFAULT_SIZE
#define SECS_COMPONENT_POOL_DEFAULT_SIZE 64
#endif


template <typename InstanceID>
EngineInstance<InstanceID>::EntityManager::Entity::Entity(const EntityID &id) :
    m_id(id) {
}

template <typename InstanceID>
template <typename C, typename... Args>
typename EngineInstance<InstanceID>::EntityManager::Entity &
EngineInstance<InstanceID>::EntityManager::Entity::assign(Args&& ...args) {
    EntityManager::get_component_storage<C>().put(m_id, std::forward<Args>(args)...);
    return *this;
}

template <typename InstanceID>
template <typename C>
typename EngineInstance<InstanceID>::EntityManager::Entity &
EngineInstance<InstanceID>::EntityManager::Entity::assign(const C &component) {
    EntityManager::get_component_storage<C>().put(m_id, component);
    return *this;
}

template <typename InstanceID>
template <typename C>
typename EngineInstance<InstanceID>::EntityManager::Entity &
EngineInstance<InstanceID>::EntityManager::Entity::assign(C &&component) {
    EntityManager::get_component_storage<C>().put(m_id, std::forward<C>(component));
    return *this;
}

template <typename InstanceID>
template <typename C>
typename EngineInstance<InstanceID>::EntityManager::Entity &
EngineInstance<InstanceID>::EntityManager::Entity::remove() {
    get_component_storage<C>().remove(m_id);
    return *this;
}

template <typename InstanceID>
template <typename C>
bool EngineInstance<InstanceID>::EntityManager::Entity::has() {
    return get_component_storage<C>().contains(m_id);
}

template <typename InstanceID>
template <typename C>
C &EngineInstance<InstanceID>::EntityManager::Entity::get() {
    return get_component_storage<C>().get(m_id);
}

template <typename InstanceID>
template <typename C>
C *EngineInstance<InstanceID>::EntityManager::Entity::find() {
    return has<C>() ? &get_component_storage<C>().get(m_id) : nullptr;
}

template <typename InstanceID>
EntityID EngineInstance<InstanceID>::EntityManager::Entity::get_id() const {
    return m_id;
}



template <typename InstanceID>
EntityID EngineInstance<InstanceID>::EntityManager::create() {
    // TODO: implementation
    return EntityID();
}

template <typename InstanceID>
void EngineInstance<InstanceID>::EntityManager::destroy(const EntityID &id) {
    for (auto destructor : s_component_destructors) {
        destructor(id);
    }
    // TODO: recycle entity ID
}

template <typename InstanceID>
typename EngineInstance<InstanceID>::EntityManager::Entity
EngineInstance<InstanceID>::EntityManager::entity(const EntityID &id) {
    return Entity(id);
}

template <typename InstanceID>
template <typename C>
SparseMap<C> EngineInstance<InstanceID>::EntityManager::create_component_storage() {
    s_component_destructors.append(+[](const EntityID &id) -> void {
        get_component_storage<C>().remove(id);
    });
    return SparseMap<C>(SECS_COMPONENT_POOL_DEFAULT_SIZE);
}

template <typename InstanceID>
template <typename C>
SparseMap<C> &EngineInstance<InstanceID>::EntityManager::get_component_storage() {
    static SparseMap<C> storage = create_component_storage<C>();
    return storage;
}


#endif // SECS_ENTITY_MANAGER_INL
