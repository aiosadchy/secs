#ifndef SECS_ENGINE_HPP
#define SECS_ENGINE_HPP

#include <memory>
#include <vector>

#include <utl/non_copyable.hpp>

#include "SECS/entity.hpp"
#include "SECS/entity_view.hpp"
#include "SECS/collections/component_pool.hpp"
#include "SECS/collections/entity_pool.hpp"
#include "SECS/component.hpp"

template <typename Family>
class Engine {
public:
    using Components = Component<Family>;

    Engine();
    ~Engine();

    NON_COPYABLE(Engine)

    [[nodiscard]] Entity::ID create();
    void destroy(const Entity::ID &entity);
    [[nodiscard]] bool is_alive(const Entity::ID &entity) const;

    template <typename C, typename... Args>
    auto &assign(const Entity::ID &entity, Args&& ...args);

    template <typename... C>
    [[nodiscard]] bool has(const Entity::ID &entity) const;

    template <typename... C>
    [[nodiscard]] decltype(auto) get(const Entity::ID &entity);

    template <typename... C>
    [[nodiscard]] decltype(auto) get(const Entity::ID &entity) const;

    template <typename... C>
    [[nodiscard]] decltype(auto) find(const Entity::ID &entity);

    template <typename... C>
    [[nodiscard]] decltype(auto) find(const Entity::ID &entity) const;

    template <typename... C>
    void remove(const Entity::ID &entity);

    template <typename... C>
    [[nodiscard]] View<Engine<Family>, C...> iterate();

    template <typename... C>
    [[nodiscard]] View<const Engine<Family>, C...> iterate() const;

    [[nodiscard]] View<EntityPool> iterate();

    [[nodiscard]] View<const EntityPool> iterate() const;

private:
    template <typename T>
    typename Components::template Pool<T> &get_component_pool();

    template <typename T>
    const typename Components::template Pool<T> &get_component_pool() const;

    std::vector<std::unique_ptr<typename Components::AbstractPool>> m_component_pools;
    EntityPool m_entity_pool;

};

#endif // SECS_ENGINE_HPP
