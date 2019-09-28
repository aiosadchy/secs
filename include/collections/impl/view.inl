//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_VIEW_INL
#define SECS_VIEW_INL

#include "../view.hpp"

template <typename Iterator>
template <typename B, typename E>
View<Iterator>::View(B &&begin, E &&end) :
    m_begin(std::forward<B>(begin)),
    m_end(std::forward<E>(end)) {}

template <typename Iterator>
Iterator View<Iterator>::begin() const {
    return m_begin;
}

template <typename Iterator>
Iterator View<Iterator>::end() const {
    return m_end;
}

#endif // SECS_VIEW_INL
