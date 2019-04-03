#ifndef SECS_EVENT_MANAGER_HPP
#define SECS_EVENT_MANAGER_HPP


#include <queue>
#include "callback_chain.hpp"
#include "engine.hpp"

template <typename App>
class Engine<App>::EventManager {
public:
    template <typename E>
    static CallbackID<E> subscribe(const typename CallbackChain<E>::Function &f);

    template <typename E>
    static bool unsibscribe(const CallbackID<E> &id);

    template <typename E, typename ...Args>
    static void raise(Args&& ...args);

private:
    template <typename E>
    static CallbackChain<E> &callbacks();

    template <typename E>
    static void processNext();

    template <typename E>
    static std::queue<E> &eventQueue();

    static std::queue<void(*)()> &handlerQueue();

};


#endif // SECS_EVENT_MANAGER_HPP
