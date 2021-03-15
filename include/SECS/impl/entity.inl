#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL

#include "SECS/entity.hpp"

Entity::ID::ID()
    : ID(INDEX_MASK, VERSION_MASK) {
}

Entity::ID::ID(Index index, Index version)
    : m_data(((UnderlyingType(index  ) & INDEX_MASK  ) << INDEX_OFFSET  )
           | ((UnderlyingType(version) & VERSION_MASK) << VERSION_OFFSET)) {
}

Index Entity::ID::get_index() const {
    return (m_data >> INDEX_OFFSET) & INDEX_MASK;
}

Index Entity::ID::get_version() const {
    return (m_data >> VERSION_OFFSET) & VERSION_MASK;
}

bool Entity::ID::operator==(const Entity::ID &another) const {
    return (get_index() == another.get_index()) && (get_version() == another.get_version());
}

bool Entity::ID::operator!=(const Entity::ID &another) const {
    return !(*this == another);
}

#endif // SECS_ENTITY_INL
