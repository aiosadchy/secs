#ifndef SECS_DEBUG_INL
#define SECS_DEBUG_INL

#include "SECS/debug.hpp"

#include <tuple>
#include <iostream>

#include <utl/type_traits.hpp>


namespace secs {

namespace debug {

SECS_IF_DEBUG (

template <typename T>
IteratorTracer<T>::IteratorTracer(T &iterator)
    : m_iterator(iterator) {
    // TODO: register iterator
    std::cout << " -- tracing " << &m_iterator << std::endl;
}

template <typename T>
IteratorTracer<T>::~IteratorTracer() {
    // TODO: unregister iterator
    std::cout << " -- finished tracing " << &m_iterator << std::endl;
}

template <typename T>
IteratorTracer<T> &IteratorTracer<T>::operator=(const IteratorTracer &) {
    return *this;
}

template <typename T>
IteratorTracer<T> &IteratorTracer<T>::operator=(IteratorTracer &&) noexcept {
    return *this;
}

template <typename T>
Entity IteratorTracer<T>::get_current_entity() {
    using R = std::decay_t<decltype(*m_iterator)>;
    if constexpr (utl::type_traits::IS_SPECIALIZATION<R, std::tuple>) {
        return std::get<0>(*m_iterator);
    } else {
        return *m_iterator;
    }
}

) // SECS_IF_DEBUG

} // namespace debug

} // namespace secs

#endif // SECS_DEBUG_INL
