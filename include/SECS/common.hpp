#ifndef SECS_COMMON_HPP
#define SECS_COMMON_HPP

using Index = unsigned int;

#ifndef NDEBUG
    #define SECS_DEBUG
#endif

namespace secs {

#ifndef SECS_DEBUG
    constexpr bool DEBUG = false;
#else
    constexpr bool DEBUG = true;
#endif

} // namespace secs

#endif // SECS_COMMON_HPP
