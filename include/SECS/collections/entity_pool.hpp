#ifndef SECS_ENTITY_POOL_HPP
#define SECS_ENTITY_POOL_HPP

#include <type_traits>
#include <vector>

#include "SECS/common.hpp"
#include "SECS/entity.hpp"
#include "SECS/utility.hpp"


namespace secs {

class EntityPool {
private:
    class EndGuard {};

    template <typename Collection>
    class GenericIterator {
    public:
        GenericIterator();
        GenericIterator(Collection &collection, const EntityPool &pool);

        SECS_IF_DEBUG (
            GenericIterator(const GenericIterator &another);
            GenericIterator(GenericIterator &&another) noexcept;
            GenericIterator &operator=(const GenericIterator &another) = default;
            GenericIterator &operator=(GenericIterator &&another) noexcept = default;
        )

        GenericIterator &operator++();
        Entity operator*();
        bool operator!=(EndGuard end) const;

    private:
        void find_next_entity();
        void step();
        bool reached_end() const;

        Index m_index;
        Collection *m_collection;
        const EntityPool *m_pool;

        SECS_IF_DEBUG (
            debug::IteratorTracker<GenericIterator<Collection>> m_tracker = *this;
        )

    };

public:
    using Iterator      = GenericIterator<std::vector<Entity>>;
    using ConstIterator = GenericIterator<const std::vector<Entity>>;
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

} // namespace secs

#endif // SECS_ENTITY_POOL_HPP
