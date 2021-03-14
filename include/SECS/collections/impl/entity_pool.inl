#ifndef SECS_ENTITY_POOL_INL
#define SECS_ENTITY_POOL_INL

#include "SECS/collections/entity_pool.hpp"


EntityPool::EntityPool(Index default_capacity)
    : m_pool() {
    m_pool.reserve(default_capacity);
}

EntityPool::~EntityPool() {
}

Entity::ID EntityPool::create() {
    // TODO: implementation
    return m_pool.emplace_back(m_pool.size(), 0);
}

void EntityPool::destroy(const Entity::ID &entity) {
    // TODO: implementation
    if (!is_alive(entity)) {
        return;
    }
    m_pool[entity.get_index()] = Entity::ID();
}

bool EntityPool::is_alive(const Entity::ID &entity) const {
    Index index = entity.get_index();
    return (m_pool.size() > index) && (m_pool[index].get_index() == index);
}


#endif // SECS_ENTITY_POOL_INL
