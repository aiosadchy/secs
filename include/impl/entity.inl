#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL


#include "../entity.hpp"

const Meta::Entity::Controller *Entity::getController() const {
    const unsigned char *raw = reinterpret_cast<const unsigned char *>(this);
    const unsigned char offset = *(raw - 1);
    return reinterpret_cast<const Meta::Entity::Controller *>(raw - offset);
}


template <typename... Components>
template <typename T>
inline Meta::Component::Controller<T> *Entity::ComponentSet<PPack<Components...>>::get() {
    return TypeSet<Meta::Component::Controller<Components> * ...>::template get<Meta::Component::Controller<T> *>();
}


template <typename ...C>
template <typename ...Args>
inline Entity::With<C...>::With(Args&& ...args) : m_components() {
    static_assert(PPack<C...>::isUnique(),
                  "Component types in entity must be unique");
    static_assert((std::is_base_of<Component, C>::value && ...),
                  "Component classes in entity must be derived from Component");

    using DefaultTypes = typename ComponentTypes::template Subtract<Args...>::Result;

    const Meta::Entity::Controller *controller = getController();

    findComponents(ComponentTypes(), controller->getComponents());
    initialize<Args...>(Reference(controller), DefaultTypes::instance(), std::forward<Args>(args)...);
}

template <typename ...C>
inline Entity::With<C...>::~With() {
    (m_components.template get<C>()->destroy(), ...);
}

template <typename ...C>
template <typename T>
inline T &Entity::With<C...>::get() {
    return m_components.template get<T>()->getComponent();
}

template <typename ...C>
template <typename ...Components>
inline void Entity::With<C...>::findComponents(PPack<Components...>, const Meta::Component::SafePtr *components) {
    if constexpr (PPack<Components...>::size() != 0) {
        using CType = typename PPack<Components...>::First;
        CType *component = components->get<CType>();
        m_components.template get<CType> = component;
        findComponents(typename PPack<Components...>::Tail::Result::instance(), components + 1);
    }
}

template <typename ...C>
template <typename ...Args, typename ...Default>
inline void Entity::With<C...>::initialize(const Reference &entity, PPack<Default...>, Args&& ...args) {
    (m_components.template get<Args>()->init(entity, std::forward<Args>(args)), ...);
    (m_components.template get<Default>()->init(entity), ...);
}

#endif // SECS_ENTITY_INL
