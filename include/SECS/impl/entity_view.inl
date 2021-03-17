#ifndef SECS_ENTITY_VIEW_INL
#define SECS_ENTITY_VIEW_INL

#include "SECS/entity_view.hpp"

#include <algorithm>

template <typename E, typename... C>
template <bool CONST>
EntityView<E, C...>::GenericIterator<CONST>::GenericIterator()
    : m_engine(nullptr)
    , m_iterator() {
}

template <typename E, typename... C>
template <bool CONST>
template <typename P>
EntityView<E, C...>::GenericIterator<CONST>::GenericIterator(E &engine, P &pool)
    : m_engine(&engine)
    , m_iterator(std::make_pair(pool.begin(), pool.end())) {
    find_next_entity();
}

template <typename E, typename... C>
template <bool CONST>
auto &EntityView<E, C...>::GenericIterator<CONST>::operator++() {
    step();
    find_next_entity();
    return *this;
}

template <typename E, typename... C>
template <bool CONST>
decltype(auto) EntityView<E, C...>::GenericIterator<CONST>::operator*() {
    Entity::ID entity = get_current_entity();
    return std::tuple_cat(
        std::tuple<Entity::ID>(entity),
        std::forward_as_tuple(m_engine->template get<C>(entity)...)
    );
}

template <typename E, typename... C>
template <bool CONST>
bool EntityView<E, C...>::GenericIterator<CONST>::operator!=(const EndGuard &) const {
    return !reached_end();
}


template <typename E, typename... C>
template <bool CONST>
Entity::ID EntityView<E, C...>::GenericIterator<CONST>::get_current_entity() {
    return std::visit([](auto &pair) { return *pair.first; }, m_iterator);
}

template <typename E, typename... C>
template <bool CONST>
void EntityView<E, C...>::GenericIterator<CONST>::find_next_entity() {
    while (!reached_end() && !m_engine->template has<C...>(get_current_entity())) {
        step();
    }
}

template <typename E, typename... C>
template <bool CONST>
void EntityView<E, C...>::GenericIterator<CONST>::step() {
    std::visit([](auto &pair) { ++pair.first; }, m_iterator);
}

template <typename E, typename... C>
template <bool CONST>
bool EntityView<E, C...>::GenericIterator<CONST>::reached_end() const {
    return !std::visit([](auto &pair) { return pair.first != pair.second; }, m_iterator);
}


template <typename E, typename... C>
EntityView<E, C...>::EntityView(E &engine, Pool<C> & ...pool)
    : m_engine(&engine)
    , m_pools(&pool...) {
}

template <typename E, typename... C>
typename EntityView<E, C...>::Iterator EntityView<E, C...>::begin() {
    std::size_t shortest_pool = get_shortest_pool_index(std::make_index_sequence<sizeof...(C)>());
    return get_pool_iterator<Iterator>(
        shortest_pool,
        *this,
        std::make_index_sequence<sizeof...(C)>()
    );
}

template <typename E, typename... C>
typename EntityView<E, C...>::End EntityView<E, C...>::end() {
    return End();
}

template <typename E, typename... C>
typename EntityView<E, C...>::ConstIterator EntityView<E, C...>::begin() const {
    std::size_t shortest_pool = get_shortest_pool_index(std::make_index_sequence<sizeof...(C)>());
    return get_pool_iterator<ConstIterator>(
        shortest_pool,
        *this,
        std::make_index_sequence<sizeof...(C)>()
    );
}

template <typename E, typename... C>
typename EntityView<E, C...>::ConstEnd EntityView<E, C...>::end() const {
    return ConstEnd();
}

template <typename E, typename... C>
template <typename R, typename V, std::size_t First, std::size_t... Rest>
R EntityView<E, C...>::get_pool_iterator(std::size_t i, V &v, std::index_sequence<First, Rest...>) {
    if (i == 0) {
        return R(*v.m_engine, *std::get<First>(v.m_pools));
    }
    if constexpr (sizeof...(Rest) > 0) {
        return get_pool_iterator<R>(i - 1, v, std::index_sequence<Rest...>());
    } else {
        return EntityView::Iterator();
    }
}

template <typename E, typename... C>
template <std::size_t... N>
std::size_t EntityView<E, C...>::get_shortest_pool_index(std::index_sequence<N...>) const {
    int a[sizeof...(N)] = {};
    (..., (a[N] = std::get<N>(m_pools)->size()));
    return std::min_element(std::begin(a), std::end(a)) - std::begin(a);
}


template <typename Family, typename... C>
View<Engine<Family>, C...>::View(
    Engine<Family> &engine,
    typename Base::template Pool<C> &... pool
)
    : Base(engine, pool...) {
}


template <typename Family, typename... C>
View<const Engine<Family>, C...>::View(
    const Engine<Family> &engine,
    typename Base::template Pool<C> &... pool
)
    : Base(engine, pool...) {
}

#endif // SECS_ENTITY_VIEW_INL
