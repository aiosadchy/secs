#ifndef SECS_EVENT_HPP
#define SECS_EVENT_HPP

#include <memory>
#include <type_traits>

#include <utl/non_constructible.hpp>
#include <utl/type_id.hpp>

#include "SECS/common.hpp"
#include "SECS/component.hpp"
#include "SECS/entity.hpp"


namespace secs {

template <typename Family>
class Event {
public:
    template <typename E>
    using Decay = std::decay_t<E>;

    using ICallbacks = ICallbackStorage;

    template <typename E>
    using Callbacks = CallbackStorage<Decay<E>>;

    using CallbacksHandle = std::unique_ptr<ICallbacks>;

    using TypeID = utl::TypeID<Event<Family>, Index, Decay, utl::init::TypeID::LAZY>;

    class Metadata : public LinkedMetadata<Metadata, Decay> {
    public:
        inline CallbacksHandle create_callbacks_storage() const;
        inline TypeID get_type_id() const;

    private:
        friend class utl::TypeInfo<Metadata, Decay, utl::init::TypeInfo::STATIC>;
        using Base = LinkedMetadata<Metadata, Decay>;
        using CreateStorage = CallbacksHandle ();

        template <typename T>
        explicit Metadata(typename Metadata::template Initializer<T>);

        CreateStorage * const m_create_storage;
        const TypeID m_type_id;

    };

private:
    using Components = Component<Family>;

    class EntityEvent {
    public:
        explicit EntityEvent(Entity entity);
        Entity get_entity() const;

    private:
        Entity m_entity;

    };

    template <typename C>
    class ComponentEvent : public EntityEvent {
    public:
        ComponentEvent(Entity entity, typename Components::template Pool<C> &pool);
        C &get_component();
        const C &get_component() const;

    protected:
        typename Components::template Pool<C> *m_pool;

    };

    class EntityCreatedEvent : public EntityEvent {
    public:
        explicit EntityCreatedEvent(Entity entity);

    };

    class EntityDestroyedEvent : public EntityEvent {
    public:
        EntityDestroyedEvent(Entity entity, EntityPool &pool);
        void finalize();

    private:
        EntityPool *m_pool;

    };

    template <typename C>
    class EntityGotComponentEvent : public ComponentEvent<C> {
    public:
        EntityGotComponentEvent(Entity entity, typename Components::template Pool<C> &pool);

    };

    template <typename C>
    class EntityLostComponentEvent : public ComponentEvent<C> {
    public:
        EntityLostComponentEvent(Entity entity, typename Components::template Pool<C> &pool);
        void finalize();

    };

    template <typename C>
    using ComponentDecay = typename Components::template Decay<C>;

public:
    using EntityCreated = EntityCreatedEvent;

    using EntityDestroyed = EntityDestroyedEvent;

    template <typename C>
    using EntityGotComponent = EntityGotComponentEvent<ComponentDecay<C>>;

    template <typename C>
    using EntityLostComponent = EntityLostComponentEvent<ComponentDecay<C>>;

    static typename Metadata::View view();

    NON_CONSTRUCTIBLE(Event)

};


} // namespace secs

#endif // SECS_EVENT_HPP
