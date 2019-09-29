//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_VIEW_INL
#define SECS_VIEW_INL

#include "../view.hpp"

template <typename C>
View<C>::View(C &collection) :
    m_begin(collection.begin()),
    m_end(collection.end()) {}

template <typename C>
typename View<C>::Begin View<C>::begin() const {
    return m_begin;
}

template <typename C>
typename View<C>::End View<C>::end() const {
    return m_end;
}

#endif // SECS_VIEW_INL
