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
        Entity operator*();
        bool operator!=(const EndGuard &end) const;

    private:
        void find_next_entity();
        bool reached_end() const;

        Iterator m_iterator;
        Iterator m_end;
        const EntityPool *m_pool;

    };

public:
    using Iterator      = GenericIterator<std::vector<Entity>::iterator>;
    using ConstIterator = GenericIterator<std::vector<Entity>::const_iterator>;
    using End      = EndGuard;
    using ConstEnd = EndGuard;

    inline explicit EntityPool(Index default_capacity, Index recycle_period);
    inline ~EntityPool();

    inline Iterator begin();
    inline End end();

    inline ConstIterator begin() const;
    inline ConstEnd end() const;

    [[nodiscard]] inline Entity create();
    inline void destroy(Entity entity);
    [[nodiscard]] inline bool is_alive(Entity entity) const;

    inline static Index get_index(Entity entity) noexcept;
    inline static Index get_version(Entity entity) noexcept;

private:
    std::vector<Entity> m_pool;
    struct {
        Index first;
        Index last;
        Index count;
    } m_free_list;
    Index m_recycle_period;

};

#endif // SECS_ENTITY_POOL_HPP
