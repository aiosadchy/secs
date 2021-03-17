#ifndef SECS_ENTITY_VIEW_HPP
#define SECS_ENTITY_VIEW_HPP

#include <type_traits>
#include <utility>
#include <variant>

#include "SECS/common.hpp"
#include "SECS/component.hpp"
#include "SECS/entity.hpp"


template <typename E, typename... C>
class EntityView {
private:
    using Components = typename E::Components;

    class EndGuard {};

    template <typename T, bool FORCE_CONST = std::is_const_v<E>>
    using Pool = std::conditional_t<
        FORCE_CONST,
        const typename Components::template Pool<T>,
        typename Components::template Pool<T>
    >;

    template <bool CONST>
    class GenericIterator {
    private:
        template <typename T>
        using PoolIteratorPair = std::pair<
            decltype(std::declval<Pool<T, CONST>>().begin()),
            decltype(std::declval<Pool<T, CONST>>().end())
        >;

    public:
        GenericIterator();

        template <typename P>
        GenericIterator(E &engine, P &pool);

        auto &operator++();
        decltype(auto) operator*();
        bool operator!=(const EndGuard &end) const;

    private:
        Entity::ID get_current_entity();
        void find_next_entity();
        void step();
        bool reached_end() const;

        E *m_engine;
        std::variant<PoolIteratorPair<C>...> m_iterator;

    };

public:
    using Iterator      = GenericIterator<std::is_const_v<E>>;
    using ConstIterator = GenericIterator<true>;
    using End      = EndGuard;
    using ConstEnd = EndGuard;

    explicit EntityView(E &engine, Pool<C> & ...pool);

    Iterator begin();
    End end();

    ConstIterator begin() const;
    ConstEnd end() const;

private:
    template <typename R, typename V, std::size_t First, std::size_t... Rest>
    static R get_pool_iterator(Index i, V &v, std::index_sequence<First, Rest...>);

    template <std::size_t... N>
    Index get_shortest_pool_index(std::index_sequence<N...>) const;

    E *m_engine;
    std::tuple<Pool<C> * ...> m_pools;

};

#endif // SECS_ENTITY_VIEW_HPP
