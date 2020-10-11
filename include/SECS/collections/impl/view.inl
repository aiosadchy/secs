#ifndef SECS_VIEW_INL
#define SECS_VIEW_INL

#include "SECS/collections/view.hpp"

template <typename C>
View<C>::View(C &collection) :
        m_collection(collection) {
}

template <typename C>
decltype(auto) View<C>::begin() {
    return std::begin(m_collection);
}

template <typename C>
decltype(auto) View<C>::end() {
    return std::end(m_collection);
}

#endif // SECS_VIEW_INL
