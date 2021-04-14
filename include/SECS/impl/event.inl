#ifndef SECS_EVENT_INL
#define SECS_EVENT_INL

#include "SECS/event.hpp"


namespace secs {

template <typename Family>
Event<Family>::EntityEvent::EntityEvent(Entity entity)
    : m_entity(entity) {
}

template <typename Family>
Entity Event<Family>::EntityEvent::get_entity() const {
    return m_entity;
}


template <typename Family>
template <typename C>
Event<Family>::ComponentEvent<C>::ComponentEvent(
        Entity entity,
        typename Components::template Pool<C> &pool)
    : EntityEvent(entity)
    , m_pool(&pool) {
}

template <typename Family>
template <typename C>
C &Event<Family>::ComponentEvent<C>::get_component() {
    return m_pool->get(EntityEvent::get_entity());
}

template <typename Family>
template <typename C>
const C &Event<Family>::ComponentEvent<C>::get_component() const {
    return m_pool->get(EntityEvent::get_entity());
}


template <typename Family>
Event<Family>::EntityCreatedEvent::EntityCreatedEvent(Entity entity)
    : EntityEvent(entity)  {
}


template <typename Family>
Event<Family>::EntityDestroyedEvent::EntityDestroyedEvent(Entity entity, EntityPool &pool)
    : EntityEvent(entity)
    , m_pool(&pool) {
}

template <typename Family>
void Event<Family>::EntityDestroyedEvent::finalize() {
    if (m_pool != nullptr) {
        m_pool->destroy(EntityEvent::get_entity());
    }
}


template <typename Family>
template <typename C>
Event<Family>::EntityGotComponentEvent<C>::EntityGotComponentEvent(
        Entity entity,
        typename Components::template Pool<C> &pool
)
    : ComponentEvent<C>(entity, pool) {
}


template <typename Family>
template <typename C>
Event<Family>::EntityLostComponentEvent<C>::EntityLostComponentEvent(
        Entity entity,
        typename Components::template Pool<C> &pool
)
    : ComponentEvent<C>(entity, pool) {
}

template <typename Family>
template <typename C>
void Event<Family>::EntityLostComponentEvent<C>::finalize() {
    ComponentEvent<C>::m_pool->remove(ComponentEvent<C>::get_entity());
}

} // namespace secs

#endif // SECS_EVENT_INL
