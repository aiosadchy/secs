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
    static const Index index = Initializer<T>::s_index;
    return TypeID(index);
}

template <typename Family, typename Index>
template <typename T>
Index TypeID<Family, Index>::get_index() {
    static const Index index = s_family_size++;
    return index;
}

template <typename Family, typename Index>
Index TypeID<Family, Index>::family_size() {
    return s_family_size;
}

template <typename Family, typename Index>
TypeID<Family, Index>::TypeID(const Index &id) :
    m_index(id) {
}

template <typename Family, typename Index>
Index TypeID<Family, Index>::s_family_size = Index(0);

template <typename Family, typename Index>
template <typename T>
const Index TypeID<Family, Index>::Initializer<T>::s_index =
            TypeID<Family, Index>::template get_index<T>();

#endif // SECS_TYPE_ID_INL
