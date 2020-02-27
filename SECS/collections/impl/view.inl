//
// Created by aiosadchy on 29/09/2019.
//

#ifndef SECS_VIEW_INL
#define SECS_VIEW_INL

#include <type_traits>
#include "SECS/collections/view.hpp"

namespace implementation {
    template <typename T>
    struct IsViewImpl {
        static const bool value = false;
    };

    template <typename T>
    struct IsViewImpl<View<T>> {
        static const bool value = true;
    };

    template <typename T>
    struct IsView {
        static const bool value = IsViewImpl<typename std::decay<T>::type>::value;
    };
}

template <typename C>
View<C>::View(C &collection) :
    m_collection(collection) {
    static_assert(!implementation::IsView<C>::value);
}

template <typename C>
auto View<C>::begin() {
    return m_collection.begin();
}

template <typename C>
auto View<C>::end() {
    return m_collection.end();
}

#endif // SECS_VIEW_INL
