#ifndef SECS_RANGE_INL
#define SECS_RANGE_INL

#include "SECS/utility/range.hpp"

template <typename T>
Range<T>::Iterator::Iterator(const T &initial_value) :
        m_value(initial_value) {
}

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
    return m_value != another.m_value;
}


template <typename T>
Range<T>::Range(const T &length) :
        m_begin(),
        m_end(length) {
}

template <typename T>
Range<T>::Range(const T &start, const T &end) :
        m_begin(start),
        m_end(end) {
}

template <typename T>
typename Range<T>::Iterator Range<T>::begin() const {
    return Iterator(m_begin);
}

template <typename T>
typename Range<T>::Iterator Range<T>::end() const {
    return Iterator(m_end);
}

#endif // SECS_RANGE_INL
