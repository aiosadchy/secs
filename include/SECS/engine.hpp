#ifndef SECS_ENGINE_HPP
#define SECS_ENGINE_HPP

#include <memory>
#include <vector>

#include <utl/non_copyable.hpp>

#include "SECS/entity.hpp"
#include "SECS/collections/component_pool.hpp"
#include "SECS/collections/entity_pool.hpp"
#include "SECS/component.hpp"

template <typename Family>
class Engine {
public:
    Engine();
    ~Engine();

    UTL_NON_COPYABLE(Engine)

    [[nodiscard]] Entity::ID create();
    void destroy(const Entity::ID &entity);
    [[nodiscard]] bool is_alive(const Entity::ID &entity) const;

    template <typename Component, typename ...Args>
    auto &assign(const Entity::ID &entity, Args&& ...args);

    template <typename ...Component>
    [[nodiscard]] bool has(const Entity::ID &entity) const;

    template <typename ...Component>
    [[nodiscard]] decltype(auto) get(const Entity::ID &entity);

    template <typename ...Component>
    [[nodiscard]] decltype(auto) get(const Entity::ID &entity) const;

    template <typename ...Component>
    [[nodiscard]] decltype(auto) find(const Entity::ID &entity);

    template <typename ...Component>
    [[nodiscard]] decltype(auto) find(const Entity::ID &entity) const;

    template <typename ...Component>
    void remove(const Entity::ID &entity);

private:
    using Components = Component<Family>;

    template <typename T>
    auto &get_component_pool();

    template <typename T>
    const auto &get_component_pool() const;

    std::vector<std::unique_ptr<AbstractComponentPool>> m_component_pools;
    EntityPool m_entity_pool;

};

#endif // SECS_ENGINE_HPP

#include "impl/engine.inl"
