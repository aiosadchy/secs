#ifndef SECS_EVENT_MANAGER_HPP
#define SECS_EVENT_MANAGER_HPP

#include <deque>
#include <queue>
#include <utility>

#include "SECS/event.hpp"


namespace secs {

template <typename Family>
class EventManager {
public:
    using Events = Event<Family>;

    inline EventManager();
    inline ~EventManager();

    template <typename E>
    void handle(const E &event) const;

    // TODO: overloads
    template <typename E, typename F>
    CallbackID register_callback(F &&function);

private:
    template <typename T>
    using Queue = std::queue<T, std::deque<T>>;

    using ProcessEvent = void ();

    template <typename T>
    typename Events::template Callbacks<T> &get_storage();

    std::vector<typename Events::CallbacksHandle> m_callbacks;

    inline static thread_local Queue<ProcessEvent *> s_unprocessed = {};

    template <typename E>
    inline static thread_local Queue<E> s_events = {};

};

} // namespace secs

#endif // SECS_EVENT_MANAGER_HPP
