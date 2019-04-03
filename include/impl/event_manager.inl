#ifndef SECS_EVENT_MANAGER_INL
#define SECS_EVENT_MANAGER_INL


#include "../event_manager.hpp"

template <typename App>
template <typename E>
CallbackID<E> Engine<App>::EventManager::subscribe(const typename CallbackChain<E>::Function &f) {
    return callbacks<E>().add(f);
}

template <typename App>
template <typename E>
bool Engine<App>::EventManager::unsibscribe(const CallbackID<E> &id) {
    return callbacks<E>().remove(id);
}

template <typename App>
template <typename E, typename ...Args>
void Engine<App>::EventManager::raise(Args&& ...args) {
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

template <typename App>
template <typename E>
CallbackChain<E> &Engine<App>::EventManager::callbacks() {
    static CallbackChain<E> chain = CallbackChain<E>();
    return chain;
}

template <typename App>
template <typename E>
void Engine<App>::EventManager::processNext() {
    callbacks<E>().fire(eventQueue<E>().front());
    eventQueue<E>().pop();
}

template <typename App>
template <typename E>
std::queue<E> &Engine<App>::EventManager::eventQueue() {
    static std::queue<E> queue = std::queue<E>();
    return queue;
}

template <typename App>
std::queue<void (*)()> &Engine<App>::EventManager::handlerQueue() {
    static std::queue<void (*)()> queue = std::queue<void (*)()>();
    return queue;
}


#endif // SECS_EVENT_MANAGER_INL
