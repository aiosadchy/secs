#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL

#include "SECS/entity.hpp"

Entity::ID::ID() :
        m_index(std::numeric_limits<Index>::max()),
        m_version(std::numeric_limits<Index>::max()) {
}

Entity::ID::ID(Index index, Index version) :
        m_index(index),
        m_version(version) {
}

Index Entity::ID::index() const {
    return m_index;
}

Index Entity::ID::version() const {
    return m_version;
}


template <typename Component, typename... Args>
Component &Entity::Handle::assign(Args &&... args) {
    return Component();
}

template <typename... Component>
bool Entity::Handle::has() const {
    return false;
}

template <typename... Component>
decltype(auto) Entity::Handle::get() {
    return nullptr;
}

template <typename... Component>
decltype(auto) Entity::Handle::get() const {
    return nullptr;
}

template <typename... Component>
decltype(auto) Entity::Handle::find() {
    return nullptr;
}

template <typename... Component>
decltype(auto) Entity::Handle::find() const {
    return nullptr;
}

template <typename... Component>
void Entity::Handle::remove() {

}

#endif // SECS_ENTITY_INL
