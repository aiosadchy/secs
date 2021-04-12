#ifndef SECS_EVENT_MANAGER_HPP
#define SECS_EVENT_MANAGER_HPP

#include <deque>
#include <queue>

#include "SECS/event.hpp"


namespace secs {

namespace detail {

// class

} // namespace detail

template <typename Family>
class EventManager {
public:
    using Events = Event<Family>;

    inline EventManager();
    inline ~EventManager();

    template <typename E>
    void handle(const E &event) const;

    template <typename E, typename F>
    void subscribe(F &&function);

private:
    template <typename T>
    using Queue = std::queue<T, std::deque<T>>;

    using ProcessEvent = void ();

    // std::vector<std::vector<std::function<void >>>

    inline static thread_local Queue<ProcessEvent *> s_unprocessed = {};

    template <typename E>
    inline static thread_local Queue<E> s_events = {};

};

} // namespace secs

#endif // SECS_EVENT_MANAGER_HPP
