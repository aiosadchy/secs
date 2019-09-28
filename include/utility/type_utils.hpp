//
// Created by aiosadchy on 28/09/2019.
//

#ifndef SECS_TYPE_UTILS_HPP
#define SECS_TYPE_UTILS_HPP

template <typename C>
struct remove_reference {
    using T = C;
};

template <typename C>
struct remove_reference<C &> {
    using T = C;
};

template <typename C>
struct remove_reference<C &&> {
    using T = C;
};


template <typename T>
typename remove_reference<T>::T &&move(T object) {
    return static_cast<T &&>(object);
}

#endif // SECS_TYPE_UTILS_HPP
