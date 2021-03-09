#ifndef SECS_UTILITY_HPP
#define SECS_UTILITY_HPP

#include <type_traits>
#include "SECS/common.hpp"

template <typename T, typename U>
T fast_dynamic_cast(U object) {
    static_assert(
        std::is_pointer_v<U> || std::is_reference_v<U>,
        "Function argument is neither a reference nor a pointer"
    );
    if constexpr (secs::DEBUG) {
        return dynamic_cast<T>(object);
    } else {
        if constexpr (std::is_pointer_v<U>) {
            static_assert(std::is_pointer_v<T>);
            return reinterpret_cast<T>(object);
        } else {
            static_assert(std::is_reference_v<T>);
            return *reinterpret_cast<std::remove_reference_t<T> *>(&object);
        }
    }
}

#endif // SECS_UTILITY_HPP
