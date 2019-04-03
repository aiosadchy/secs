#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL


#include "../entity.hpp"

template <typename ...Components>
template <typename ...Args>
Entity<Components...>::Entity(Args&&... args) : m_components() {
    static_assert(PPack<Components...>::isUnique(), "Component types in Entity must be unique");
    using DefaultTypes = typename ComponentTypes::template Subtract<PPack<Args...>>::Result;

    // An entity must be created inside Holder only
    unsigned char *raw = reinterpret_cast<unsigned char *>(this);
    // Previous byte will always contain byte offset
    // to the start of corresponding Metadata structure
    unsigned char offset = *(raw - 1);
    EntityRoutine::Metadata *metadata = reinterpret_cast<EntityRoutine::Metadata *>(raw - offset);

    findComponents(ComponentTypes(), &(metadata->components));
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
void Entity<Components...>::findComponents(PPack<Types...>, const EntityRoutine::ComponentReference *components) {
    using CType = typename PPack<Types...>::First;
    if constexpr (PPack<Types...>::size() != 0) {
        CType *component = components->get<CType>();
        if (component == nullptr) {
            throw std::runtime_error("An entity has been created manually instead of use EntityManager::create");
        }
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
