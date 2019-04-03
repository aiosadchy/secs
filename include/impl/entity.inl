#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL


#include "../entity.hpp"

inline EntityHandle::EntityHandle(const Meta::Entity::Controller *c) :
                    m_controller(c), m_generation((c != nullptr) ? c->getGeneration() : 0) {}

inline bool EntityHandle::isAlive() const {
    return (m_controller != nullptr)
            && (m_generation == m_controller->getGeneration())
            && (m_controller->isAlive());
}

inline EntityHandle::operator bool() const {
    return isAlive();
}

inline const Meta::Entity::Controller *EntityHandle::getController() const {
    return isAlive() ? m_controller : nullptr;
}


template <typename E>
inline Handle<E>::Handle(const Meta::Entity::Controller *c) :
                    EntityHandle(c), m_entity(getController()->template getEntity<E>()) {}

template <typename E>
inline E *Handle<E>::operator->() {
    return isAlive() ? m_entity : nullptr;
}

template <typename E>
inline const E *Handle<E>::operator->() const {
    return isAlive() ? m_entity : nullptr;
}


template <typename ...Components>
template <typename ...Args>
inline Entity<Components...>::Entity(Args&&... args) : m_components() {
    static_assert(PPack<Components...>::isUnique(), "Component types in Entity must be unique");
    using DefaultTypes = typename ComponentTypes::template Subtract<PPack<Args...>>::Result;

    unsigned char *raw = reinterpret_cast<unsigned char *>(this);
    unsigned char offset = *(raw - 1);
    const Meta::Entity::Controller *controller = reinterpret_cast<Meta::Entity::Controller *>(raw - offset);

    findComponents(ComponentTypes(), controller->getComponents());
    initialize<Args...>(DefaultTypes(), std::forward<Args>(args)...);
}

template <typename ...Components>
template <typename C>
inline C &Entity<Components...>::get() {
    static_assert(ComponentTypes::template contains<C>(), "Entity does not contain requested component");
    return *m_components.template get<C *>();
}

template <typename ...Components>
template <typename ...Types>
inline void Entity<Components...>::findComponents(PPack<Types...>, const Meta::Component::SafePtr *components) {
    if constexpr (PPack<Types...>::size() != 0) {
        using CType = typename PPack<Types...>::First;
        CType *component = components->get<CType>();
        m_components.template get<CType *> = component;
        findComponents(typename PPack<Types...>::Tail::Result(), components + 1);
    }
}

template <typename ...Components>
template <typename ...Args, typename ...Default>
inline void Entity<Components...>::initialize(PPack<Default...>, Args&&... args) {
    (new (m_components.template get<Args *>()) Args(std::forward<Args>(args)), ...);
    (new (m_components.template get<Default *>()) Default(), ...);
}

#endif // SECS_ENTITY_INL
