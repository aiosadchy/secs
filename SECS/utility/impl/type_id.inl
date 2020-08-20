#ifndef SECS_TYPE_ID_INL
#define SECS_TYPE_ID_INL

#include <limits>
#include "SECS/utility/type_id.hpp"

template <typename Family, typename Index>
TypeID<Family, Index>::TypeID() :
        m_index(std::numeric_limits<Index>::max()) {
}

template <typename Family, typename Index>
Index TypeID<Family, Index>::index() const {
    return m_index;
}

template <typename Family, typename Index>
TypeID<Family, Index>::TypeID(Index id) :
        m_index(id) {
}

template <typename Family, typename Index>
bool TypeID<Family, Index>::operator==(const TypeID &another) const {
    return m_index == another.m_index;
}

template <typename Family, typename Index>
bool TypeID<Family, Index>::operator!=(const TypeID &another) const {
    return !(another == *this);
}

template <typename Family, typename Index>
template <typename T>
TypeID<Family, Index> TypeID<Family, Index>::get() {
    static const Index index = s_family_size++;
    return TypeID(index);
}

template <typename Family, typename Index>
Index TypeID<Family, Index>::s_family_size = Index(0);

#endif // SECS_TYPE_ID_INL
