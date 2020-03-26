#ifndef SECS_ENTITY_MANAGER_HPP
#define SECS_ENTITY_MANAGER_HPP

#include "SECS/collections/sparse_map.hpp"
#include "engine.hpp"
#include "entity_id.hpp"

template <typename InstanceID>
class EngineInstance<InstanceID>::EntityManager {
public:
    class Entity {
    public:
        explicit Entity(const EntityID &id);

        template <typename C, typename ...Args>
        Entity &assign(Args&& ...args);

        template <typename C>
        Entity &assign(const C &component);

        template <typename C>
        Entity &assign(C &&component);

        template <typename C>
        Entity &remove();

        template <typename C>
        bool has();

        template <typename C>
        C &get();

        template <typename C>
        C *find();

        EntityID get_id() const;

    private:
        EntityID m_id;

    };

    static EntityID create();
    static void destroy(const EntityID &id);

    static Entity entity(const EntityID &id);

    static EntityManager instance;

private:
    EntityManager() = default;

    template <typename C>
    static SparseMap<C> create_component_storage();

    template <typename C>
    static SparseMap<C> &get_component_storage();

    static Vector<void(*)(const EntityID &)> s_component_destructors;

};

#ifndef SECS_ENTITY_MANAGER_INL
#include "impl/entity_manager.inl"
#endif

#endif // SECS_ENTITY_MANAGER_HPP
