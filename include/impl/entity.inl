#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL


#include "../entity.hpp"

template <typename ...Components>
template <typename ...Args>
Entity<Components...>::Entity(Args&&... args) : m_components() {
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
C &Entity<Components...>::get() {
    static_assert(ComponentTypes::template contains<C>(), "Entity does not contain requested component");
    return *m_components.template get<C *>();
}

template <typename ...Components>
template <typename ...Types>
void Entity<Components...>::findComponents(PPack<Types...>, const Meta::Component::SafePtr *components) {
    if constexpr (PPack<Types...>::size() != 0) {
        using CType = typename PPack<Types...>::First;
        CType *component = components->get<CType>();
        m_components.template get<CType *> = component;
        findComponents(typename PPack<Types...>::Tail::Result(), components + 1);
    }
}

template <typename ...Components>
template <typename ...Args, typename ...Default>
void Entity<Components...>::initialize(PPack<Default...>, Args&&... args) {
    (new (m_components.template get<Args *>()) Args(std::forward<Args>(args)), ...);
    (new (m_components.template get<Default *>()) Default(), ...);
}

#endif // SECS_ENTITY_INL
