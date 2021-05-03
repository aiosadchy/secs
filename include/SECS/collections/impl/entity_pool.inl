#ifndef SECS_ENTITY_POOL_INL
#define SECS_ENTITY_POOL_INL

#include "SECS/collections/entity_pool.hpp"

#include <utility>


namespace secs {

template <typename Collection>
EntityPool::GenericIterator<Collection>::GenericIterator()
    : m_index(Index(0) - Index(1))
    , m_collection(nullptr)
    , m_pool(nullptr) {
}

template <typename Collection>
EntityPool::GenericIterator<Collection>::GenericIterator(
        Collection &collection,
        const EntityPool &pool
)
    : m_index(static_cast<Index>(collection.size()) - Index(1))
    , m_collection(&collection)
    , m_pool(&pool) {
    find_next_entity();
}

SECS_IF_DEBUG (

template <typename Collection>
EntityPool::GenericIterator<Collection>::GenericIterator(const GenericIterator<Collection> &another)
    : GenericIterator() {
    *this = another;
}

template <typename Collection>
EntityPool::GenericIterator<Collection>::GenericIterator(GenericIterator<Collection> &&another) noexcept
    : GenericIterator() {
    *this = std::move(another);
}

) // SECS_IF_DEBUG

template <typename Collection>
typename EntityPool::GenericIterator<Collection> &EntityPool::GenericIterator<Collection>::operator++() {
    step();
    find_next_entity();
    return *this;
}

template <typename Collection>
Entity EntityPool::GenericIterator<Collection>::operator*() {
    return (*m_collection)[m_index];
}

template <typename Collection>
bool EntityPool::GenericIterator<Collection>::operator!=(EndGuard) const {
    return !reached_end();
}

template <typename Collection>
void EntityPool::GenericIterator<Collection>::find_next_entity() {
    while (!reached_end() && !m_pool->is_alive(*(*this))) {
        step();
    }
}

template <typename Collection>
void EntityPool::GenericIterator<Collection>::step() {
    m_index -= Index(1);
}

template <typename Collection>
bool EntityPool::GenericIterator<Collection>::reached_end() const {
    return (m_index + Index(1)) == 0;
}


EntityPool::EntityPool(Index default_capacity, Index recycle_period)
    : m_pool()
    , m_free_list({0, 0, 1})
    , m_recycle_period((recycle_period > 1) ? recycle_period : 1) {
    if (default_capacity > 0) {
        m_pool.reserve(default_capacity);
    }
    m_pool.emplace_back(Entity::null);
}

EntityPool::~EntityPool() = default;

EntityPool::Iterator EntityPool::begin() {
    return Iterator(m_pool, *this);
}

EntityPool::End EntityPool::end() {
    return End();
}

EntityPool::ConstIterator EntityPool::begin() const {
    return ConstIterator(m_pool, *this);
}

EntityPool::End EntityPool::end() const {
    return ConstEnd();
}

Entity EntityPool::create() {
    if (m_free_list.count > m_recycle_period) {
        Index created = m_free_list.first;
        m_free_list.first = m_pool[created].get_index();
        m_free_list.count -= 1;
        m_pool[created] = Entity(created, m_pool[created].get_version());
        return m_pool[created];
    }
    return m_pool.emplace_back(Entity(static_cast<Index>(m_pool.size()), 0));
}

void EntityPool::destroy(Entity entity) {
    if (!is_alive(entity)) {
        return;
    }
    Index index = entity.get_index();
    Index version = entity.get_version();
    m_pool[index] = Entity(Entity::null.get_index(), version + 1);
    Entity &last_free = m_pool[m_free_list.last];
    last_free = Entity(index, last_free.get_version());
    m_free_list.last = index;
    m_free_list.count += 1;
}

bool EntityPool::is_alive(Entity entity) const {
    Index index = entity.get_index();
    return (m_pool.size() > index) && (m_pool[index] == entity);
}

Index EntityPool::get_index(Entity entity) noexcept {
    return entity.get_index();
}

Index EntityPool::get_version(Entity entity) noexcept {
    return entity.get_version();
}

} // namespace secs

#endif // SECS_ENTITY_POOL_INL
