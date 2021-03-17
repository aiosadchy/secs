#ifndef SECS_ENTITY_VIEW_HPP
#define SECS_ENTITY_VIEW_HPP

#include <type_traits>
#include <utility>
#include <variant>

#include "SECS/collections/view.hpp"
#include "SECS/entity.hpp"


template <typename Family>
class Engine;


template <typename E, typename... C>
class EntityView {
protected:
    using Components = typename E::Components;

    template <typename T, bool ADD_CONST = false>
    using Pool = std::conditional_t<
        ADD_CONST || std::is_const_v<E>,
        const typename Components::template Pool<T>,
        typename Components::template Pool<T>
    >;

private:
    class EndGuard {};

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
    static R get_pool_iterator(std::size_t i, V &v, std::index_sequence<First, Rest...>);

    template <std::size_t... N>
    std::size_t get_shortest_pool_index(std::index_sequence<N...>) const;

    E *m_engine;
    std::tuple<Pool<C> * ...> m_pools;

};


template <typename Family, typename... C>
class View<Engine<Family>, C...> : public EntityView<Engine<Family>, C...> {
private:
    using Base = EntityView<Engine<Family>, C...>;

public:
    explicit View(Engine<Family> &engine, typename Base::template Pool<C> & ...pool);

};


template <typename Family, typename... C>
class View<const Engine<Family>, C...> : public EntityView<const Engine<Family>, C...> {
private:
    using Base = EntityView<const Engine<Family>, C...>;

public:
    explicit View(const Engine<Family> &engine, typename Base::template Pool<C> & ...pool);

};

#endif // SECS_ENTITY_VIEW_HPP
