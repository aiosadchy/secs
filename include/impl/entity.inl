#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL


#include "../entity.hpp"

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
