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
    return Entity::ID();
}

void EntityPool::destroy(const Entity::ID &entity) {
    // TODO: implementation
    if (!is_alive(entity)) {
        return;
    }
}

bool EntityPool::is_alive(const Entity::ID &entity) const {
    Index index = entity.get_index();
    return (m_pool.size() > index) && (m_pool[index].get_index() == index);
}


#endif // SECS_ENTITY_POOL_INL
