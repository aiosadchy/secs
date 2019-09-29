//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_RANGE_INL
#define SECS_RANGE_INL

#include "../range.hpp"

template <typename T>
inline Range<T>::Iterator::Iterator(T initial_value) :
        m_value(initial_value) {}

template <typename T>
inline T &Range<T>::Iterator::operator*() {
    return m_value;
}

template <typename T>
inline typename Range<T>::Iterator &Range<T>::Iterator::operator++() {
    ++m_value;
    return *this;
}

template <typename T>
inline bool Range<T>::Iterator::operator!=(const Iterator &another) const {
    return !(m_value == another.m_value);
}


template <typename T>
inline Range<T>::Range(const T &length) :
        m_begin(),
        m_end(length) {}

template <typename T>
inline Range<T>::Range(const T &start, const T &end) :
        m_begin(start),
        m_end(end) {}

template <typename T>
inline typename Range<T>::Iterator Range<T>::begin() {
    return Iterator(m_begin);
}

template <typename T>
inline typename Range<T>::Iterator Range<T>::end() {
    return Iterator(m_end);
}

template <typename T>
inline typename Range<T>::Iterator Range<T>::begin() const {
    return const_cast<Range<T> *>(this)->begin();
}

template <typename T>
inline typename Range<T>::Iterator Range<T>::end() const {
    return const_cast<Range<T> *>(this)->end();
}

#endif // SECS_RANGE_INL
