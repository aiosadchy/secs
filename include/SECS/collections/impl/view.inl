#ifndef SECS_VIEW_INL
#define SECS_VIEW_INL

#include "SECS/collections/view.hpp"


namespace secs {

template <typename C>
View<C>::View(C &collection)
    : m_collection(&collection) {
}

template <typename C>
auto View<C>::begin() {
    return std::begin(*m_collection);
}

template <typename C>
auto View<C>::end() {
    return std::end(*m_collection);
}

} // namespace secs

#endif // SECS_VIEW_INL
