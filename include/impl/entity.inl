#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL


#include "../entity.hpp"

EntityBase::EntityBase() : m_alive(false), m_generation(1), m_destructor(nullptr) {}

EntityBase::~EntityBase() {}

bool EntityBase::init(EntityBase::Destructor destructor) {
    if (m_alive) {
        return false;
    } else {
        m_alive = true;
        m_destructor = destructor;
        return true;
    }
}

bool EntityBase::destroy() {
    if (m_alive) {
        m_alive = false;
        m_generation += 1;
        m_destructor(this);
        m_destructor = nullptr;
        return true;
    } else {
        return false;
    }
}

EntityHandle::EntityHandle(EntityBase *base) : m_generation(0), m_entity(base) {
    if (m_entity != nullptr) {
        m_generation = m_entity->m_generation;
    }
}

bool EntityHandle::alive() const {
    return (m_entity != nullptr) && (m_generation == m_entity->m_generation) && (m_entity->m_alive);
}

template<typename ...Components>
Entity<Components...>::Entity() : m_components() {
    static_assert(PPack<Components...>::isUnique(), "Component types in Entity must be unique");
}

template<typename... Components>
template<typename T>
T *Entity<Components...>::get() {
    if constexpr (PPack<Components...>::template contains<T>()) {
        return m_components.template get<T>();
    } else {
        return nullptr;
    }
}


#endif // SECS_ENTITY_INL
