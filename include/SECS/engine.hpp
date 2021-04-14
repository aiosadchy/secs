#ifndef SECS_ENGINE_HPP
#define SECS_ENGINE_HPP

#include <memory>
#include <vector>

#include <utl/non_copyable.hpp>

#include "SECS/collections/entity_pool.hpp"
#include "SECS/collections/entity_view.hpp"
#include "SECS/component.hpp"
#include "SECS/entity.hpp"
#include "SECS/event_manager.hpp"


namespace secs {

template <typename Family>
class Engine {
public:
    using Components = Component<Family>;
    using Events     = typename EventManager<Family>::Events;

    Engine();
    ~Engine();

    NON_COPYABLE(Engine)

    [[nodiscard]] Entity create();

    void destroy(Entity entity);

    [[nodiscard]] bool is_alive(Entity entity) const;

    // TODO: overloads
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

    EventManager<Family> &get_event_manager();

    template <typename... C>
    void register_component();

private:
    using PoolHandle = std::unique_ptr<typename Components::IPool>;

    using TypeID = typename Components::TypeID;

    template <typename T>
    using Decay = typename Components::template Decay<T>;

    class Metadata : public LinkedMetadata<Metadata, Decay> {
    public:
        using CreatePool = PoolHandle (*)(Index);
        using RemoveComponent = void (*)(Engine &, Entity);

        template <typename T>
        explicit Metadata(typename Metadata::template Initializer<T>);

        inline TypeID get_type_id() const;

        const CreatePool create_pool;
        const RemoveComponent remove_component;

    private:
        const TypeID m_type_id;

    };


    template <typename T>
    typename Components::template Pool<T> &get_component_pool();

    template <typename T>
    const typename Components::template Pool<T> &get_component_pool() const;

    void register_component(const Metadata &metadata);

    std::vector<PoolHandle> m_component_pools;
    std::vector<typename Metadata::RemoveComponent> m_remove_component;
    EntityPool m_entity_pool;
    EventManager<Family> m_event_manager;

};

} // namespace secs

#endif // SECS_ENGINE_HPP
