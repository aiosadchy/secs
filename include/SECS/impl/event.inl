#ifndef SECS_EVENT_INL
#define SECS_EVENT_INL

#include "SECS/event.hpp"


namespace secs {

template <typename Family>
typename Event<Family>::CallbacksHandle Event<Family>::Metadata::create_callbacks_storage() const {
    return m_create_storage();
}

template <typename Family>
typename Event<Family>::TypeID Event<Family>::Metadata::get_type_id() const {
    return m_type_id;
}

template <typename Family>
template <typename T>
Event<Family>::Metadata::Metadata(typename Metadata::template Initializer<T>)
    : Base(this)
    , m_create_storage(
        +[]() {
            return CallbacksHandle(new CallbackStorage<T>());
        })
    , m_type_id(TypeID::template get<T>()) {
}


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
Event<Family>::ComponentEvent<C>::ComponentEvent(Entity entity, C &component)
    : EntityEvent(entity)
    , m_component(&component) {
}

template <typename Family>
template <typename C>
C &Event<Family>::ComponentEvent<C>::get_component() {
    return *m_component;
}

template <typename Family>
template <typename C>
const C &Event<Family>::ComponentEvent<C>::get_component() const {
    return *m_component;
}


template <typename Family>
Event<Family>::EntityCreatedEvent::EntityCreatedEvent(Entity entity)
    : EntityEvent(entity)  {
}


template <typename Family>
Event<Family>::EntityDestroyedEvent::EntityDestroyedEvent(Entity entity)
    : EntityEvent(entity) {
}


template <typename Family>
template <typename C>
Event<Family>::EntityGotComponentEvent<C>::EntityGotComponentEvent(Entity entity, C &component)
    : ComponentEvent<C>(entity, component) {
}


template <typename Family>
template <typename C>
Event<Family>::EntityLostComponentEvent<C>::EntityLostComponentEvent(Entity entity, C &component)
    : ComponentEvent<C>(entity, component) {
}


template <typename Family>
typename Event<Family>::Metadata::View Event<Family>::view() {
    return Metadata::view();
}

} // namespace secs

#endif // SECS_EVENT_INL
