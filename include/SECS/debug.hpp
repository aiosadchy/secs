#ifndef SECS_DEBUG_HPP
#define SECS_DEBUG_HPP

#include <utility>

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

class IIteratorTracer {
public:
    virtual ~IIteratorTracer() = default;
    virtual Entity get_current_entity() = 0;

};

template <typename T>
class IteratorTracer : public IIteratorTracer {
public:
    explicit IteratorTracer(T &iterator);
    ~IteratorTracer();

    IteratorTracer &operator=(const IteratorTracer &another);
    IteratorTracer &operator=(IteratorTracer &&another) noexcept;

    Entity get_current_entity() override;

private:
    T &m_iterator;

};

class IteratorRegistry {
public:
    UTL_NON_CONSTRUCTIBLE(IteratorRegistry)

    inline static void on_destroy(Entity entity);

    template <typename C>
    inline static void on_remove(Entity entity);

    template <typename... C>
    static void track(IIteratorTracer &tracker);

    template <typename... C>
    static void release(IIteratorTracer &tracker);

private:


};

) // SECS_IF_DEBUG


#define SECS_TRACE_ITERATOR(Iterator) SECS_IF_DEBUG (                                          \
        private:                                                                               \
            debug::IteratorTracer<Iterator> m_tracer = debug::IteratorTracer<Iterator>(*this); \
                                                                                               \
        public:                                                                                \
            Iterator(const Iterator &another)                                                  \
                : Iterator() {                                                                 \
                *this = another;                                                               \
            }                                                                                  \
                                                                                               \
            Iterator(Iterator &&another) noexcept                                              \
                : Iterator() {                                                                 \
                *this = std::move(another);                                                    \
            }                                                                                  \
                                                                                               \
            Iterator &operator=(const Iterator &) = default;                                   \
            Iterator &operator=(Iterator &&) noexcept = default;                               \
                                                                                               \
        private:                                                                               \
    )

} // namespace debug

} // namespace secs

#endif // SECS_DEBUG_HPP
