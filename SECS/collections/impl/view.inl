#ifndef SECS_VIEW_INL
#define SECS_VIEW_INL

#include <type_traits>
#include "SECS/collections/view.hpp"

namespace internal {
    template <typename T>
    struct IsView {
        static const bool value = false;
    };

    template <typename T>
    struct IsView<View<T>> {
        static const bool value = true;
    };
}

template <typename C>
View<C>::View(C &collection) :
    m_collection(collection) {
    static_assert(!internal::IsView<typename std::decay<C>::type>::value);
}

template <typename C>
auto View<C>::begin() {
    return std::begin(m_collection);
}

template <typename C>
auto View<C>::end() {
    return std::end(m_collection);
}

#endif // SECS_VIEW_INL
