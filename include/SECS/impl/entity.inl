#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL

#include "SECS/entity.hpp"

Entity::ID::ID() noexcept
    : ID(INDEX_MASK, VERSION_MASK) {
}

Entity::ID::ID(Index index, Index version) noexcept
    : m_data(((UnderlyingType(index  ) & INDEX_MASK  ) << INDEX_OFFSET  )
           | ((UnderlyingType(version) & VERSION_MASK) << VERSION_OFFSET)) {
}

Index Entity::ID::get_index() const noexcept {
    return (m_data >> INDEX_OFFSET) & INDEX_MASK;
}

Index Entity::ID::get_version() const noexcept {
    return (m_data >> VERSION_OFFSET) & VERSION_MASK;
}

bool Entity::ID::operator==(const Entity::ID &another) const noexcept {
    return (get_index() == another.get_index()) && (get_version() == another.get_version());
}

bool Entity::ID::operator!=(const Entity::ID &another) const noexcept {
    return !(*this == another);
}

#endif // SECS_ENTITY_INL
