#ifndef SECS_ENGINE_HPP
#define SECS_ENGINE_HPP

#include <vector>

#include <utl/non_copyable.hpp>

#include "SECS/entity.hpp"
#include "SECS/entity_view.hpp"
#include "SECS/collections/entity_pool.hpp"
#include "SECS/component.hpp"

template <typename Family>
class Engine {
public:
    using Components = Component<Family>;

    Engine();
    ~Engine();

    NON_COPYABLE(Engine)

    [[nodiscard]] Entity create();

    void destroy(Entity entity);

    [[nodiscard]] bool is_alive(Entity entity) const;

    template <typename C, typename... Args>
    auto &assign(Entity entity, Args&& ...args);

    template <typename... C>
    [[nodiscard]] bool has(Entity entity) const;

    template <typename... C>
    [[nodiscard]] decltype(auto) get(Entity entity);

    template <typename... C>
    [[nodiscard]] decltype(auto) get(Entity entity) const;

    template <typename... C>
    [[nodiscard]] decltype(auto) find(Entity entity);

    template <typename... C>
    [[nodiscard]] decltype(auto) find(Entity entity) const;

    template <typename... C>
    void remove(Entity entity);

    template <typename... C>
    [[nodiscard]] View<Engine<Family>, C...> view();

    template <typename... C>
    [[nodiscard]] View<const Engine<Family>, C...> view() const;

    [[nodiscard]] View<EntityPool> view();

    [[nodiscard]] View<const EntityPool> view() const;

private:
    template <typename T>
    typename Components::template Pool<T> &get_component_pool();

    template <typename T>
    const typename Components::template Pool<T> &get_component_pool() const;

    std::vector<typename Components::PoolHandle> m_component_pools;
    EntityPool m_entity_pool;

};

#endif // SECS_ENGINE_HPP
