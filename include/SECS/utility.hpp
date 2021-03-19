#ifndef SECS_UTILITY_HPP
#define SECS_UTILITY_HPP

#include <type_traits>
#include <utility>

#include "SECS/common.hpp"

template <typename T, typename U>
inline T fast_dynamic_cast(U *object) {
    static_assert(std::is_pointer_v<T>);
    if constexpr (secs::DEBUG) {
        return dynamic_cast<T>(object);
    } else {
        return reinterpret_cast<T>(object);
    }
}

template <typename T, typename U>
inline T fast_dynamic_cast(U &object) {
    static_assert(std::is_reference_v<T>);
    if constexpr (secs::DEBUG) {
        return dynamic_cast<T>(object);
    } else {
        return *reinterpret_cast<std::remove_reference_t<T> *>(&object);
    }
}


class Identity {
public:
    template <typename T>
    inline constexpr T &&operator()(T &&input) const noexcept {
        return std::forward<T>(input);
    }

};

template <typename T, bool APPLY>
using AddConst = std::conditional_t<APPLY, const T, T>;

#endif // SECS_UTILITY_HPP
