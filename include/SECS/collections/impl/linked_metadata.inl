#ifndef SECS_LINKED_METADATA_INL
#define SECS_LINKED_METADATA_INL

#include "SECS/collections/linked_metadata.hpp"


namespace secs {

template <typename Data, template <typename> typename Decay>
LinkedMetadata<Data, Decay>::Iterator::Iterator()
    : m_item(nullptr) {
}

template <typename Data, template <typename> typename Decay>
LinkedMetadata<Data, Decay>::Iterator::Iterator(const Data *item)
    : m_item(item) {
}

template <typename Data, template <typename> typename Decay>
const Data &LinkedMetadata<Data, Decay>::Iterator::operator*() const {
    return *m_item;
}

template <typename Data, template <typename> typename Decay>
const typename LinkedMetadata<Data, Decay>::Iterator &LinkedMetadata<Data, Decay>::Iterator::operator++() {
    m_item = LinkedMetadata::View::next(m_item);
    return *this;
}

template <typename Data, template <typename> typename Decay>
bool LinkedMetadata<Data, Decay>::Iterator::operator!=(const Iterator &another) const {
    return m_item != another.m_item;
}


template <typename Data, template <typename> typename Decay>
typename LinkedMetadata<Data, Decay>::Iterator LinkedMetadata<Data, Decay>::View::begin() const {
    return LinkedMetadata::Iterator(LinkedMetadata::s_head);
}

template <typename Data, template <typename> typename Decay>
typename LinkedMetadata<Data, Decay>::Iterator LinkedMetadata<Data, Decay>::View::end() const {
    return LinkedMetadata::Iterator(nullptr);
}

template <typename Data, template <typename> typename Decay>
const Data *LinkedMetadata<Data, Decay>::View::first() {
    return LinkedMetadata::s_head;
}

template <typename Data, template <typename> typename Decay>
const Data *LinkedMetadata<Data, Decay>::View::next(const Data *data) {
    return data->m_next;
}


template <typename Data, template <typename> typename Decay>
typename LinkedMetadata<Data, Decay>::View LinkedMetadata<Data, Decay>::view() {
    return LinkedMetadata::View();
}

template <typename Data, template <typename> typename Decay>
Index LinkedMetadata<Data, Decay>::get_types_count() {
    return s_registered_types;
}

template <typename Data, template <typename> typename Decay>
LinkedMetadata<Data, Decay>::LinkedMetadata(const Data *derived)
    : m_next(s_head) {
    s_head = derived;
    ++s_registered_types;
}

} // namespace secs

#endif // SECS_LINKED_METADATA_INL
