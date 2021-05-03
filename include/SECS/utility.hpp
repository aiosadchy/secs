#ifndef SECS_UTILITY_HPP
#define SECS_UTILITY_HPP

#include <type_traits>
#include <utility>

#include "SECS/common.hpp"


namespace secs {

class Identity {
public:
    template <typename T>
    inline constexpr T &&operator()(T &&input) const noexcept {
        return std::forward<T>(input);
    }

};

template <typename T, bool APPLY>
using AddConst = std::conditional_t<APPLY, const T, T>;

} // namespace secs

#endif // SECS_UTILITY_HPP
