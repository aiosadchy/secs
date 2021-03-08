#ifndef SECS_ENTITY_POOL_HPP
#define SECS_ENTITY_POOL_HPP

#include <vector>

#include "SECS/common.hpp"
#include "SECS/entity.hpp"

class EntityPool {
public:
    explicit EntityPool(Index default_capacity);
    ~EntityPool();

    [[nodiscard]] Entity::ID create();
    void destroy(const Entity::ID &entity);
    [[nodiscard]] bool is_alive(const Entity::ID &entity) const;

private:
    std::vector<Entity::ID> m_pool;

};

#endif // SECS_ENTITY_POOL_HPP

#include "impl/entity_pool.inl"
