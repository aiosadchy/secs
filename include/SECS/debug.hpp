#ifndef SECS_DEBUG_HPP
#define SECS_DEBUG_HPP

#include <utl/non_constructible.hpp>

#include "entity.hpp"


// TODO: define SECS_DEBUG with cmake option
#ifndef NDEBUG
    #define SECS_DEBUG
#endif


#ifndef SECS_DEBUG
    #define SECS_IF_DEBUG(...)
#else
    #define SECS_IF_DEBUG(...) __VA_ARGS__
#endif


namespace secs {

#ifndef SECS_DEBUG
    constexpr bool DEBUG = false;
#else
    constexpr bool DEBUG = true;
#endif


namespace debug {

SECS_IF_DEBUG (

class IIteratorTracker {
public:
    virtual ~IIteratorTracker() = default;
    virtual Entity get_current_entity() = 0;

};

template <typename T>
class IteratorTracker : public IIteratorTracker {
public:
    explicit IteratorTracker(T &iterator);
    ~IteratorTracker();

    IteratorTracker &operator=(const IteratorTracker &another);
    IteratorTracker &operator=(IteratorTracker &&another) noexcept;

    Entity get_current_entity() override;

private:
    T &m_iterator;

};

class IteratorTrackerStorage {
public:
    UTL_NON_CONSTRUCTIBLE(IteratorTrackerStorage)

    inline static void on_destroy(Entity entity);

    template <typename C>
    inline static void on_remove(Entity entity);

    template <typename... C>
    static void track(IIteratorTracker &tracker);

    template <typename... C>
    static void release(IIteratorTracker &tracker);

private:


};

) // SECS_IF_DEBUG

} // namespace debug

} // namespace secs

#endif // SECS_DEBUG_HPP
