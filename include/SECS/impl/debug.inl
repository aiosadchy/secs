#ifndef SECS_DEBUG_INL
#define SECS_DEBUG_INL

#include "SECS/debug.hpp"

#include <tuple>

#include <utl/type_traits.hpp>


namespace secs {

namespace debug {

SECS_IF_DEBUG (

template <typename T>
IteratorTracker<T>::IteratorTracker(T &iterator)
    : m_iterator(iterator) {
    // TODO: register iterator
}

template <typename T>
IteratorTracker<T>::~IteratorTracker() {
    // TODO: unregister iterator
}

template <typename T>
IteratorTracker<T> &IteratorTracker<T>::operator=(const IteratorTracker &) {
    return *this;
}

template <typename T>
IteratorTracker<T> &IteratorTracker<T>::operator=(IteratorTracker &&) noexcept {
    return *this;
}

template <typename T>
Entity IteratorTracker<T>::get_current_entity() {
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
