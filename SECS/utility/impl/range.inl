#ifndef SECS_RANGE_INL
#define SECS_RANGE_INL

#include "SECS/utility/range.hpp"

template <typename T>
Range<T>::Iterator::Iterator(const T &initial_value, const T &step) :
        m_value(initial_value),
        m_step(step) {
}

template <typename T>
T &Range<T>::Iterator::operator*() {
    return m_value;
}

template <typename T>
typename Range<T>::Iterator &Range<T>::Iterator::operator++() {
    m_value += m_step;
    return *this;
}

template <typename T>
bool Range<T>::Iterator::operator!=(const Iterator &another) const {
    return m_value != another.m_value;
}


template <typename T>
Range<T>::Range(const T &length) :
        m_begin(0),
        m_end(length),
        m_step(1) {
}

template <typename T>
Range<T>::Range(const T &begin, const T &end, const T &step) :
        m_begin(begin),
        m_end(end),
        m_step(step) {
    T step_count = (m_end - m_begin) / m_step;
    if (((m_end - m_begin) % m_step) != T(0)) {
        step_count += T(1);
    }
    if (step_count < T(0)) {
        step_count = 0;
    }
    m_end = m_begin + step_count * m_step;
}

template <typename T>
typename Range<T>::Iterator Range<T>::begin() const {
    return Iterator(m_begin, m_step);
}

template <typename T>
typename Range<T>::Iterator Range<T>::end() const {
    return Iterator(m_end, m_step);
}

#endif // SECS_RANGE_INL
