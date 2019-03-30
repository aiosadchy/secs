#include "event_manager.hpp"

template<typename Application>
template<typename E>
CallbackID<E> EventManager<Application>::subscribe(const typename CallbackChain<E>::Function &f) {
    return callbacks<E>().add(f);
}

template<typename Application>
template<typename E>
bool EventManager<Application>::unsibscribe(const CallbackID<E> &id) {
    return callbacks<E>().remove(id);
}

template<typename Application>
template<typename E, typename ...Args>
void EventManager<Application>::raise(Args&& ...args) {
    auto &handlers = handlerQueue();
    eventQueue<E>().emplace(std::forward<Args>(args)...);
    handlers.emplace(&processNext<E>);
    if (handlers.size() == 1) {
        while (!handlers.empty()) {
            handlers.front()();
            handlers.pop();
        }
    }
}

template<typename Application>
template<typename E>
CallbackChain<E> &EventManager<Application>::callbacks() {
    static CallbackChain<E> chain = CallbackChain<E>();
    return chain;
}

template<typename Application>
template<typename E>
void EventManager<Application>::processNext() {
    callbacks<E>().fire(eventQueue<E>().front());
    eventQueue<E>().pop();
}

template<typename Application>
template<typename E>
std::queue<E> &EventManager<Application>::eventQueue() {
    static std::queue<E> queue = std::queue<E>();
    return queue;
}

template<typename Application>
std::queue<void (*)()> &EventManager<Application>::handlerQueue() {
    static std::queue<void (*)()> queue = std::queue<void (*)()>();
    return queue;
}
