#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL


#include "../entity.hpp"

inline Entity::Reference::Reference(const Meta::Entity::Controller *c) : m_controller(c), m_generation(0) {
    if (m_controller != nullptr) {
        m_generation = m_controller->getGeneration();
    }
}

inline bool Entity::Reference::isValid() const {
    return (m_controller != nullptr)
        && (m_controller->getGeneration() == m_generation)
        && (m_controller->isAlive());
}

inline Entity::Reference::operator bool() const {
    return isValid();
}

inline void Entity::Reference::invalidate() {
    m_controller = nullptr;
}


template <typename E>
inline Entity::Handle<E>::Handle(const Meta::Entity::Controller *c) : Reference(c), m_entity(nullptr) {
    if ((c == nullptr) || (!c->isOfType<E>())) {
        invalidate();
    } else {
        m_entity = c->getEntity<E>();
    }
}

template <typename E>
inline E *Entity::Handle<E>::operator->() {
    return getEntity();
}

template <typename E>
inline const E *Entity::Handle<E>::operator->() const {
    return getEntity();
}

template <typename E>
inline E &Entity::Handle<E>::operator*() {
    return *getEntity();
}

template <typename E>
inline const E &Entity::Handle<E>::operator*() const {
    return *getEntity();
}

template <typename E>
inline E *Entity::Handle<E>::getEntity() {
    return isValid() ? m_entity : nullptr;
}

template <typename E>
inline const E *Entity::Handle<E>::getEntity() const {
    return const_cast<Handle<E> *>(this)->getEntity();
}


template <typename ...C>
template <typename ...Args>
inline Entity::With<C...>::With(Args&&... args) : m_components() {
    static_assert(PPack<C...>::isUnique(),
                  "Component types in entity must be unique");
    static_assert((std::is_base_of<Component, C>::value && ...),
                  "Component classes in entity must be derived from Component");

    using DefaultTypes = typename ComponentPack::template Subtract<PPack<Args...>>::Result;

    unsigned char *raw = reinterpret_cast<unsigned char *>(this);
    unsigned char offset = *(raw - 1);
    const Meta::Entity::Controller *controller = reinterpret_cast<Meta::Entity::Controller *>(raw - offset);

    findComponents(ComponentPack(), controller->getComponents());
    initialize<Args...>(DefaultTypes(), std::forward<Args>(args)...);
}

template <typename ...C>
template <typename T>
inline T &Entity::With<C...>::get() {
    static_assert(ComponentPack::template contains<T>(), "Entity does not contain requested component");
    return *m_components.template get<T *>();
}

template <typename ...C>
template <typename ...Types>
inline void Entity::With<C...>::findComponents(PPack<Types...>, const Meta::Component::SafePtr *components) {
    if constexpr (PPack<Types...>::size() != 0) {
        using CType = typename PPack<Types...>::First;
        CType *component = components->get<CType>();
        m_components.template get<CType *> = component;
        findComponents(typename PPack<Types...>::Tail::Result(), components + 1);
    }
}

template <typename ...C>
template <typename ...Args, typename ...Default>
inline void Entity::With<C...>::initialize(PPack<Default...>, Args&&... args) {
    (new (m_components.template get<Args *>()) Args(std::forward<Args>(args)), ...);
    (new (m_components.template get<Default *>()) Default(), ...);
}

#endif // SECS_ENTITY_INL
