#ifndef SECS_ENTITY_MANAGER_INL
#define SECS_ENTITY_MANAGER_INL

#include "SECS/entity_manager.hpp"

#ifndef SECS_COMPONENT_POOL_DEFAULT_SIZE
#define SECS_COMPONENT_POOL_DEFAULT_SIZE 64
#endif


Entity::ID EntityManager::create() {
    // TODO: implementation
    return Entity::ID();
}

void EntityManager::destroy(const Entity::ID &id) {
    // TODO: implementation
}

Entity::Reference EntityManager::entity(const Entity::ID &id) {
    // TODO: implementation
    return Entity::Reference(id);
}


#endif // SECS_ENTITY_MANAGER_INL
