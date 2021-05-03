#ifndef SECS_ENTITY_VIEW_INL
#define SECS_ENTITY_VIEW_INL

#include "SECS/collections/entity_view.hpp"

#include <algorithm>


namespace secs {

namespace detail {

template <typename E, typename... C>
EntityView<E, C...>::Iterator::Iterator() noexcept
    : m_engine(nullptr)
    , m_iterator() {
}

template <typename E, typename... C>
template <typename P>
EntityView<E, C...>::Iterator::Iterator(E &engine, P &pool)
    : m_engine(&engine)
    , m_iterator(std::make_pair(pool.begin(), pool.end())) {
    find_next_entity();
}

template <typename E, typename... C>
typename EntityView<E, C...>::Iterator &EntityView<E, C...>::Iterator::operator++() {
    step();
    find_next_entity();
    return *this;
}

template <typename E, typename... C>
decltype(auto) EntityView<E, C...>::Iterator::operator*() {
    Entity entity = get_current_entity();
    return std::tuple_cat(
        std::tuple<Entity>(entity),
        std::forward_as_tuple(m_engine->template get<C>(entity)...)
    );
}

template <typename E, typename... C>
bool EntityView<E, C...>::Iterator::operator!=(End) const {
    return !reached_end();
}


template <typename E, typename... C>
Entity EntityView<E, C...>::Iterator::get_current_entity() {
    return std::visit([](auto &pair) { return *pair.first; }, m_iterator);
}

template <typename E, typename... C>
void EntityView<E, C...>::Iterator::find_next_entity() {
    while (!reached_end() && !m_engine->template has<C...>(get_current_entity())) {
        step();
    }
}

template <typename E, typename... C>
void EntityView<E, C...>::Iterator::step() {
    std::visit([](auto &pair) { ++pair.first; }, m_iterator);
}

template <typename E, typename... C>
bool EntityView<E, C...>::Iterator::reached_end() const {
    return !std::visit([](auto &pair) { return pair.first != pair.second; }, m_iterator);
}


template <typename E, typename... C>
EntityView<E, C...>::EntityView(E &engine, Pool <C> &...pool)
    : m_engine(&engine)
    , m_pools(&pool...) {
}

template <typename E, typename... C>
typename EntityView<E, C...>::Iterator EntityView<E, C...>::begin() {
    auto indices = std::make_integer_sequence<Index, sizeof...(C)>();
    Index shortest_pool = get_shortest_pool_index(indices);
    return get_pool_iterator(shortest_pool, indices);
}

template <typename E, typename... C>
typename EntityView<E, C...>::End EntityView<E, C...>::end() {
    return End();
}

template <typename E, typename... C>
template <Index I, Index... Rest>
typename EntityView<E, C...>::Iterator EntityView<E, C...>::get_pool_iterator(
    Index i,
    Indices<I, Rest...>
) {
    if (i == 0) {
        return Iterator(*m_engine, *std::get<I>(m_pools));
    }
    if constexpr (sizeof...(Rest) > 0) {
        return get_pool_iterator(i - 1, Indices<Rest...>());
    } else {
        return EntityView::Iterator();
    }
}

template <typename E, typename... C>
template <Index... I>
Index EntityView<E, C...>::get_shortest_pool_index(Indices<I...>) const {
    int a[sizeof...(I)] = {};
    (..., (a[I] = std::get<I>(m_pools)->size()));
    return std::min_element(std::begin(a), std::end(a)) - std::begin(a);
}

} // namespace detail

} // namespace secs

#endif // SECS_ENTITY_VIEW_INL
