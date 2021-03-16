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


EntityPool::EntityPool(Index default_capacity)
    : m_pool() {
    m_pool.reserve(default_capacity);
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
    // TODO: implementation
    return m_pool.emplace_back(m_pool.size(), 0);
}

void EntityPool::destroy(const Entity::ID &entity) {
    // TODO: implementation
    if (!is_alive(entity)) {
        return;
    }
    m_pool[entity.get_index()] = Entity::ID();
}

bool EntityPool::is_alive(const Entity::ID &entity) const {
    Index index = entity.get_index();
    return (m_pool.size() > index) && (m_pool[index].get_index() == index);
}

#endif // SECS_ENTITY_POOL_INL
