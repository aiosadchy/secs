#ifndef SECS_TYPE_ID_INL
#define SECS_TYPE_ID_INL

#include <limits>
#include "SECS/utility/type_id.hpp"

template <typename Family, typename IDType>
TypeID<Family, IDType>::TypeID() :
    m_id(std::numeric_limits<IDType>::max()) {
}

template <typename Family, typename IDType>
IDType TypeID<Family, IDType>::get_id() const {
    return m_id;
}

template <typename Family, typename IDType>
template <typename T>
TypeID<Family, IDType> TypeID<Family, IDType>::get() {
    const static IDType type_id = s_next_type_id++;
    return TypeID(type_id);
}

template <typename Family, typename IDType>
TypeID<Family, IDType>::TypeID(IDType id) :
    m_id(id) {
}

template <typename Family, typename IDType>
bool TypeID<Family, IDType>::operator==(const TypeID &another) const {
    return m_id == another.m_id;
}

template <typename Family, typename IDType>
bool TypeID<Family, IDType>::operator!=(const TypeID &another) const {
    return !(another == *this);
}

template <typename Family, typename IDType>
IDType TypeID<Family, IDType>::s_next_type_id = IDType();

#endif // SECS_TYPE_ID_INL
