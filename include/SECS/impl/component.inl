#ifndef SECS_COMPONENT_INL
#define SECS_COMPONENT_INL

#include "SECS/component.hpp"


namespace secs {

template <typename Family>
typename Component<Family>::PoolHandle Component<Family>::Metadata::create_pool(Index capacity) const {
    return m_create_pool(capacity);
}

template <typename Family>
typename Component<Family>::TypeID Component<Family>::Metadata::get_type_id() const {
    return m_type_id;
}

template <typename Family>
template <typename T>
Component<Family>::Metadata::Metadata(typename Metadata::template Initializer<T>)
    : Base(this)
    , m_create_pool(
        +[](Index initial_capacity) {
            return PoolHandle(new Pool<T>(initial_capacity));
        })
    , m_type_id(TypeID::template get<T>()) {
}


template <typename Family>
typename Component<Family>::Metadata::View Component<Family>::view() {
    return Metadata::view();
}

} // namespace secs

#endif // SECS_COMPONENT_INL
