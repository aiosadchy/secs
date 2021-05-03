#ifndef SECS_DEBUG_HPP
#define SECS_DEBUG_HPP

// TODO: define SECS_DEBUG with cmake option
#ifndef NDEBUG
    #define SECS_DEBUG
#endif


#ifndef SECS_DEBUG
    #define SECS_IF_DEBUG(...)

    #define SECS_FAST_DYNAMIC_CAST reinterpret_cast

#else
    #define SECS_IF_DEBUG(...) __VA_ARGS__

    #define SECS_FAST_DYNAMIC_CAST dynamic_cast

#endif


namespace secs {

#ifndef SECS_DEBUG
    constexpr bool DEBUG = false;
#else
    constexpr bool DEBUG = true;
#endif

} // namespace secs

#endif // SECS_DEBUG_HPP
