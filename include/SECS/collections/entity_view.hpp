#ifndef SECS_ENTITY_VIEW_HPP
#define SECS_ENTITY_VIEW_HPP

#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include "SECS/collections/view.hpp"
#include "SECS/entity.hpp"
#include "SECS/utility.hpp"


namespace secs {

template <typename Family>
class Engine;


namespace detail {

// TODO: single component specialization
template <typename E, typename... C>
class EntityView {
protected:
    using Components = typename E::Components;

    template <typename T>
    using Pool = AddConst<typename Components::template Pool<T>, std::is_const_v<E>>;

public:
    class End {};

    class Iterator {
    private:
        template <typename T>
        using PoolIteratorPair = std::pair<
            decltype(std::declval<Pool<T>>().begin()),
            decltype(std::declval<Pool<T>>().end())
        >;

    public:
        Iterator();

        template <typename P>
        Iterator(E &engine, P &pool);

        SECS_IF_DEBUG (
            Iterator(const Iterator &another);
            Iterator(Iterator &&another) noexcept;
            Iterator &operator=(const Iterator &another) = default;
            Iterator &operator=(Iterator &&another) noexcept = default;
        )

        Iterator &operator++();
        decltype(auto) operator*();
        bool operator!=(End end) const;

    private:
        Entity get_current_entity();
        void find_next_entity();
        void step();
        bool reached_end() const;

        E *m_engine;
        std::variant<PoolIteratorPair<C>...> m_iterator;

        SECS_IF_DEBUG (
            debug::IteratorTracker<Iterator> m_tracker = *this;
        )

    };

    explicit EntityView(E &engine, Pool<C> & ...pool);

    Iterator begin();
    End end();

private:
    template <Index... I>
    using Indices = std::integer_sequence<Index, I...>;

    template <Index T, Index... Rest>
    Iterator get_pool_iterator(Index i, Indices<T, Rest...>);

    template <Index... I>
    Index get_shortest_pool_index(Indices<I...>) const;

    E *m_engine;
    std::tuple<Pool<C> * ...> m_pools;

};

} // namespace detail


template <typename Family, typename... C>
SECS_VIEW_SPECIALIZATION(detail::EntityView, Engine<Family>, C...);

template <typename Family, typename... C>
SECS_VIEW_SPECIALIZATION(detail::EntityView, const Engine<Family>, C...);

} // namespace secs

#endif // SECS_ENTITY_VIEW_HPP
