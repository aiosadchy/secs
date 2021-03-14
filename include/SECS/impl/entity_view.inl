#ifndef SECS_ENTITY_VIEW_INL
#define SECS_ENTITY_VIEW_INL

#include "SECS/entity_view.hpp"

#include <algorithm>

#include "SECS/engine.hpp"

template <typename Engine, typename... C>
template <bool IS_CONST>
EntityView<Engine, C...>::GenericIterator<IS_CONST>::GenericIterator()
    : m_engine(nullptr)
    , m_iterator()
    , m_end() {
}

template <typename Engine, typename... C>
template <bool IS_CONST>
template <typename P>
EntityView<Engine, C...>::GenericIterator<IS_CONST>::GenericIterator(Engine &engine, P &pool)
    : m_engine(&engine)
    , m_iterator(pool.begin())
    , m_end(pool.end()) {
    find_next_entity();
}

template <typename Engine, typename... C>
template <bool IS_CONST>
auto &EntityView<Engine, C...>::GenericIterator<IS_CONST>::operator++() {
    step();
    find_next_entity();
    return *this;
}

template <typename Engine, typename... C>
template <bool IS_CONST>
decltype(auto) EntityView<Engine, C...>::GenericIterator<IS_CONST>::operator*() {
    Entity::ID entity = get_current_entity();
    return std::tuple_cat(
        std::make_tuple(entity),
        std::forward_as_tuple(m_engine->template get<C>(entity)...)
    );
}

template <typename Engine, typename... C>
template <bool IS_CONST>
bool EntityView<Engine, C...>::GenericIterator<IS_CONST>::operator!=(const EndGuard &) const {
    return !reached_end();
}


template <typename Engine, typename... C>
template <bool IS_CONST>
Entity::ID EntityView<Engine, C...>::GenericIterator<IS_CONST>::get_current_entity() const {
    // TODO: fix hardcoded entity version
    return std::visit([](auto &iterator) { return Entity::ID(*iterator, 0); }, m_iterator);
}

template <typename Engine, typename... C>
template <bool IS_CONST>
void EntityView<Engine, C...>::GenericIterator<IS_CONST>::find_next_entity() {
    while (!reached_end() && !m_engine->template has<C...>(get_current_entity())) {
        step();
    }
}

template <typename Engine, typename... C>
template <bool IS_CONST>
void EntityView<Engine, C...>::GenericIterator<IS_CONST>::step() {
    std::visit([](auto &iterator) { ++iterator; }, m_iterator);
}

template <typename Engine, typename... C>
template <bool IS_CONST>
bool EntityView<Engine, C...>::GenericIterator<IS_CONST>::reached_end() const {
    return !(m_iterator != m_end);
}


template <typename Engine, typename... C>
EntityView<Engine, C...>::EntityView(Engine &engine)
    : m_engine(&engine) {
}

template <typename Engine, typename... C>
typename EntityView<Engine, C...>::Iterator EntityView<Engine, C...>::begin() {
    Index shortest_pool = get_shortest_pool_index(*m_engine);
    return get_pool_iterator_by_index<Iterator, C...>(*m_engine, shortest_pool);
}

template <typename Engine, typename... C>
typename EntityView<Engine, C...>::End EntityView<Engine, C...>::end() {
    return End();
}

template <typename Engine, typename... C>
typename EntityView<Engine, C...>::ConstIterator EntityView<Engine, C...>::begin() const {
    Index shortest_pool = get_shortest_pool_index(*m_engine);
    return get_pool_iterator_by_index<ConstIterator, C...>(*m_engine, shortest_pool);
}

template <typename Engine, typename... C>
typename EntityView<Engine, C...>::ConstEnd EntityView<Engine, C...>::end() const {
    return ConstEnd();
}

template <typename Engine, typename... C>
template <typename R, typename First, typename... Rest>
R EntityView<Engine, C...>::get_pool_iterator_by_index(Engine &engine, Index index) {
    if (index == 0) {
        return R(engine, engine.template get_component_pool<First>());
    }
    if constexpr (sizeof...(Rest) > 0) {
        return get_pool_iterator_by_index<R, Rest...>(engine, index - 1);
    } else {
        return R();
    }
}

template <typename Engine, typename... C>
Index EntityView<Engine, C...>::get_shortest_pool_index(Engine &engine) {
    int a[sizeof...(C)] = {};
    auto it = std::begin(a);
    (..., (*it++ = engine.template get_component_pool<C>().size()));
    return std::min_element(std::begin(a), std::end(a)) - std::begin(a);
}

#endif // SECS_ENTITY_VIEW_INL
