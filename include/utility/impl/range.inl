//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_RANGE_INL
#define SECS_RANGE_INL

#include "../range.hpp"

template <typename T>
Range<T>::Iterator::Iterator(T initial_value) :
        m_value(initial_value) {}

template <typename T>
T &Range<T>::Iterator::operator*() {
    return m_value;
}

template <typename T>
typename Range<T>::Iterator &Range<T>::Iterator::operator++() {
    ++m_value;
    return *this;
}

template <typename T>
bool Range<T>::Iterator::operator!=(const Iterator &another) const {
    return !(m_value == another.m_value);
}


template <typename T>
Range<T>::Range(const T &length) :
        m_begin(),
        m_end(length) {
    if (m_end < m_begin) {
        m_end = m_begin;
    }
}

template <typename T>
Range<T>::Range(const T &start, const T &end) :
        m_begin(start),
        m_end(end) {
    if (m_end < m_begin) {
        m_end = m_begin;
    }
}

template <typename T>
typename Range<T>::Iterator Range<T>::begin() {
    return Iterator(m_begin);
}

template <typename T>
typename Range<T>::Iterator Range<T>::end() {
    return Iterator(m_end);
}

template <typename T>
typename Range<T>::Iterator Range<T>::begin() const {
    return const_cast<Range<T> *>(this)->begin();
}

template <typename T>
typename Range<T>::Iterator Range<T>::end() const {
    return const_cast<Range<T> *>(this)->end();
}

#endif // SECS_RANGE_INL
