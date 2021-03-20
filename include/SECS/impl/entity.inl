#ifndef SECS_ENTITY_INL
#define SECS_ENTITY_INL

#include "SECS/entity.hpp"

namespace secs {

const Entity &Entity::get_null() noexcept {
    const static Entity null_value = Entity();
    return null_value;
}

Entity::Entity() noexcept
    : Entity(INDEX_MASK, VERSION_MASK) {
}

Entity::ID Entity::get_id() const noexcept {
    return m_data & USED_BITS_MASK;
}

bool Entity::operator==(Entity another) const noexcept {
    return get_id() == another.get_id();
}

bool Entity::operator!=(Entity another) const noexcept {
    return !(*this == another);
}

Entity::Entity(Index index, Index version) noexcept
    : m_data(((ID(index  ) & INDEX_MASK  ) << INDEX_OFFSET  )
           | ((ID(version) & VERSION_MASK) << VERSION_OFFSET)) {
}

Index Entity::get_index() const noexcept {
    return (m_data >> INDEX_OFFSET) & INDEX_MASK;
}

Index Entity::get_version() const noexcept {
    return (m_data >> VERSION_OFFSET) & VERSION_MASK;
}

} // namespace secs

#endif // SECS_ENTITY_INL
