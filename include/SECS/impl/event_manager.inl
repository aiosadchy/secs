#ifndef SECS_EVENT_MANAGER_INL
#define SECS_EVENT_MANAGER_INL

#include "SECS/event_manager.hpp"


namespace secs {

template <typename Family>
EventManager<Family>::EventManager()
    : m_callbacks() {
    for (const auto &metadata : Events::view()) {
        Index type_index = metadata.get_type_id().get_index();
        if (type_index >= m_callbacks.size()) {
            m_callbacks.resize(type_index + 1);
        }
        m_callbacks[type_index] = std::move(metadata.create_callbacks_storage());
    }
}

template <typename Family>
EventManager<Family>::~EventManager() = default;

template <typename Family>
template <typename E>
void EventManager<Family>::handle(const E &event) const {
    get_storage<E>().handle(event);
}

template <typename Family>
template <typename E, typename F>
CallbackID EventManager<Family>::register_callback(F &&function) {
    return get_storage<E>().register_callback(std::forward<F>(function));
}

template <typename Family>
template <typename T>
typename EventManager<Family>::Events::template Callbacks<T> &EventManager<Family>::get_storage() {
    Index type_index = Events::Metadata::template get<T>().get_type_id().get_index();
    typename Events::ICallbacks *storage = m_callbacks[type_index].get();
    return fast_dynamic_cast<typename Events::template Callbacks<T> &>(*storage);
}

} // namespace secs

#endif // SECS_EVENT_MANAGER_INL
