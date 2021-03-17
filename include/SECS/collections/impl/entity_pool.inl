#ifndef SECS_ENTITY_POOL_INL
#define SECS_ENTITY_POOL_INL

#include "SECS/collections/entity_pool.hpp"


template <typename Iterator>
EntityPool::GenericIterator<Iterator>::GenericIterator()
    : m_iterator()
    , m_end()
    , m_pool(nullptr) {
}

template <typename Iterator>
EntityPool::GenericIterator<Iterator>::GenericIterator(
        Iterator begin,
        Iterator end,
        const EntityPool &pool
)
    : m_iterator(begin)
    , m_end(end)
    , m_pool(&pool) {
    find_next_entity();
}

template <typename Iterator>
typename EntityPool::GenericIterator<Iterator> &EntityPool::GenericIterator<Iterator>::operator++() {
    ++m_iterator;
    find_next_entity();
    return *this;
}

template <typename Iterator>
Entity::ID EntityPool::GenericIterator<Iterator>::operator*() {
    return *m_iterator;
}

template <typename Iterator>
bool EntityPool::GenericIterator<Iterator>::operator!=(const EndGuard &) const {
    return !reached_end();
}

template <typename Iterator>
void EntityPool::GenericIterator<Iterator>::find_next_entity() {
    while (!reached_end() && !m_pool->is_alive(*m_iterator)) {
        ++m_iterator;
    }
}

template <typename Iterator>
bool EntityPool::GenericIterator<Iterator>::reached_end() const {
    return !(m_iterator != m_end);
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
    return Iterator(m_pool.begin(), m_pool.end(), *this);
}

EntityPool::End EntityPool::end() {
    return End();
}

EntityPool::ConstIterator EntityPool::begin() const {
    return ConstIterator(m_pool.begin(), m_pool.end(), *this);
}

EntityPool::End EntityPool::end() const {
    return ConstEnd();
}

Entity::ID EntityPool::create() {
    if (m_free_list.count > m_recycle_period) {
        Index created = m_free_list.first;
        m_free_list.first = m_pool[created].get_index();
        m_free_list.count -= 1;
        m_pool[created] = Entity::ID(created, m_pool[created].get_version());
        return m_pool[created];
    }
    return m_pool.emplace_back(m_pool.size(), 0);
}

void EntityPool::destroy(const Entity::ID &entity) {
    if (!is_alive(entity)) {
        return;
    }
    Index index = entity.get_index();
    Index version = entity.get_version();
    m_pool[index] = Entity::ID(Entity::null.get_index(), version + 1);
    Entity::ID &last_free = m_pool[m_free_list.last];
    last_free = Entity::ID(index, last_free.get_version());
    m_free_list.last = index;
    m_free_list.count += 1;
}

bool EntityPool::is_alive(const Entity::ID &entity) const {
    Index index = entity.get_index();
    return (m_pool.size() > index) && (m_pool[index] == entity);
}

#endif // SECS_ENTITY_POOL_INL
