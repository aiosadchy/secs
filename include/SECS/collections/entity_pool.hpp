#ifndef SECS_ENTITY_POOL_HPP
#define SECS_ENTITY_POOL_HPP

#include <type_traits>
#include <vector>

#include "SECS/common.hpp"
#include "SECS/entity.hpp"

class EntityPool {
private:
    class EndGuard {};

    template <typename Iterator>
    class GenericIterator {
    public:
        GenericIterator();
        GenericIterator(Iterator begin, Iterator end, const EntityPool &pool);

        GenericIterator &operator++();
        Entity::ID operator*();
        bool operator!=(const EndGuard &end) const;

    private:
        void find_next_entity();
        bool reached_end() const;

        Iterator m_iterator;
        Iterator m_end;
        const EntityPool *m_pool;

    };

public:
    using Iterator      = GenericIterator<std::vector<Entity::ID>::iterator>;
    using ConstIterator = GenericIterator<std::vector<Entity::ID>::const_iterator>;
    using End      = EndGuard;
    using ConstEnd = EndGuard;

    explicit EntityPool(Index default_capacity);
    ~EntityPool();

    inline Iterator begin();
    inline End end();

    inline ConstIterator begin() const;
    inline ConstEnd end() const;

    [[nodiscard]] Entity::ID create();
    void destroy(const Entity::ID &entity);
    [[nodiscard]] bool is_alive(const Entity::ID &entity) const;

private:
    std::vector<Entity::ID> m_pool;

};

#endif // SECS_ENTITY_POOL_HPP
