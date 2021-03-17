#ifndef SECS_COMPONENT_INL
#define SECS_COMPONENT_INL

#include "SECS/component.hpp"


template <typename Family>
typename Component<Family>::AbstractPool *Component<Family>::Metadata::create_pool(Index capacity) const {
    return m_pool_factory(capacity);
}

template <typename Family>
typename Component<Family>::TypeID Component<Family>::Metadata::get_type_id() const {
    return m_type_id;
}

template <typename Family>
template <typename T>
Component<Family>::Metadata::Metadata(typename Base::template Initializer<T>)
    : m_pool_factory(
        +[](Index initial_capacity) -> typename Component<Family>::AbstractPool * {
            return new ComponentPool<T>(initial_capacity);
        }
    )
    , m_type_id(TypeID::template get<T>())
    , m_next(s_head) {
    s_head = this;
}


template <typename Family>
Component<Family>::Iterator::Iterator()
    : m_item(nullptr) {
}

template <typename Family>
Component<Family>::Iterator::Iterator(const Component::Metadata *item)
    : m_item(item) {
}

template <typename Family>
const typename Component<Family>::Metadata *Component<Family>::Iterator::operator->() const {
    return m_item;
}

template <typename Family>
const typename Component<Family>::Metadata &Component<Family>::Iterator::operator*() const {
    return *m_item;
}

template <typename Family>
const typename Component<Family>::Iterator &Component<Family>::Iterator::operator++() {
    m_item = m_item->m_next;
    return *this;
}

template <typename Family>
bool
Component<Family>::Iterator::operator!=(const Component::Iterator &another) const {
    return m_item != another.m_item;
}


template <typename Family>
typename Component<Family>::Iterator Component<Family>::View::begin() const {
    return Component<Family>::Iterator(Component<Family>::Metadata::s_head);
}

template <typename Family>
typename Component<Family>::Iterator Component<Family>::View::end() const {
    return Component<Family>::Iterator(nullptr);
}


template <typename Family>
typename Component<Family>::View Component<Family>::iterate() {
    return Component<Family>::View();
}

#endif // SECS_COMPONENT_INL
