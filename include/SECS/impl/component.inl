#ifndef SECS_COMPONENT_INL
#define SECS_COMPONENT_INL

#include "SECS/component.hpp"


template <typename Family>
Component<Family>::TypeID::TypeID()
    : m_index(Index(0) - Index(1)) {
}

template <typename Family>
Index Component<Family>::TypeID::get_index() const {
    return m_index;
}

template <typename Family>
bool Component<Family>::TypeID::operator==(const Component::TypeID &another) const {
    return m_index == another.m_index;
}

template <typename Family>
bool Component<Family>::TypeID::operator!=(const Component::TypeID &another) const {
    return m_index != another.m_index;
}

template <typename Family>
template <typename T>
typename Component<Family>::TypeID Component<Family>::TypeID::get() {
    return Component<Family>::Metadata::template get<T>().get_type_id();
}

template <typename Family>
Component<Family>::TypeID::TypeID(Index index)
    : m_index(index) {
}



template <typename Family>
Component<Family>::Metadata::Iterator::Iterator()
    : m_item(nullptr) {
}

template <typename Family>
Component<Family>::Metadata::Iterator::Iterator(const Component::Metadata *item)
    : m_item(item) {
}

template <typename Family>
const typename Component<Family>::Metadata *Component<Family>::Metadata::Iterator::operator->() const {
    return m_item;
}

template <typename Family>
const typename Component<Family>::Metadata &Component<Family>::Metadata::Iterator::operator*() const {
    return *m_item;
}

template <typename Family>
const typename Component<Family>::Metadata::Iterator &Component<Family>::Metadata::Iterator::operator++() {
    m_item = m_item->m_next;
    return *this;
}

template <typename Family>
bool
Component<Family>::Metadata::Iterator::operator!=(const Component::Metadata::Iterator &another) const {
    return m_item != another.m_item;
}



template <typename Family>
typename Component<Family>::Metadata::Iterator Component<Family>::Metadata::List::begin() const {
    return Component<Family>::Metadata::Iterator(Component<Family>::Metadata::s_head);
}

template <typename Family>
typename Component<Family>::Metadata::Iterator Component<Family>::Metadata::List::end() const {
    return Component<Family>::Metadata::Iterator(nullptr);
}



template <typename Family>
template <typename T>
Component<Family>::Metadata::Metadata(utl::Type<T>)
    : m_pool_factory(
    +[](Index initial_capacity) {
        return new ComponentPool<T>(initial_capacity);
    }
)
    , m_type_index(s_head == nullptr ? 0 : (s_head->m_type_index + 1))
    , m_next(s_head) {
    s_head = this;
}

template <typename Family>
AbstractComponentPool *Component<Family>::Metadata::create_pool(Index initial_capacity) const {
    return m_pool_factory(initial_capacity);
}

template <typename Family>
typename Component<Family>::TypeID Component<Family>::Metadata::get_type_id() const {
    return Component<Family>::TypeID(m_type_index);
}

template <typename Family>
Index Component<Family>::Metadata::get_registered_types_count() {
    return s_head->m_type_index + 1;
}

template <typename Family>
const typename Component<Family>::Metadata::List &Component<Family>::Metadata::iterable() {
    static Component<Family>::Metadata::List list;
    return list;
}

}

#endif // SECS_COMPONENT_INL
