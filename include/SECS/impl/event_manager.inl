#ifndef SECS_EVENT_MANAGER_INL
#define SECS_EVENT_MANAGER_INL

#include "SECS/event_manager.hpp"

#include <utl/type_traits.hpp>


namespace secs {

namespace detail {

template <class E>
class HasFinalize {
    template <typename T, T>
    struct Has;

    template <typename T>
    static std::true_type has_finalize(Has<void (E::*)(), &T::finalize> *);

    template <typename T>
    static std::false_type has_finalize(...);

public:
    static constexpr bool value = decltype(has_finalize<E>(nullptr))::value;

};

} // namespace detail


template <typename Family>
EventManager<Family>::EventManager()
    : m_callbacks() {
    for (const auto &metadata : Metadata::view()) {
        register_event(metadata);
    }
}

template <typename Family>
EventManager<Family>::~EventManager() = default;

template <typename Family>
template <typename E>
void EventManager<Family>::handle(E &&event) {
    process_event<E>(std::forward<E>(event));
}

template <typename Family>
template <typename E, typename... Args>
void EventManager<Family>::handle(Args &&... args) {
    process_event<E>(std::forward<Args>(args)...);
}

template <typename Family>
template <typename F>
CallbackID EventManager<Family>::register_callback(F &&function) {
    using Event = typename utl::type_traits::Function<F>::template Argument<0>;
    return get_storage<Event>().register_callback(std::forward<F>(function));
}

template <typename Family>
template <typename... E>
void EventManager<Family>::register_event() {
    (register_event(Metadata::template get<E>()), ...);
}


template <typename Family>
template <typename T>
EventManager<Family>::Metadata::Metadata(typename Metadata::template Initializer<T>)
    : LinkedMetadata<Metadata, Decay>(this)
    , create_callbacks_storage(
        +[]() {
            return CallbacksHandle(new typename Events::template Callbacks<T>());
        })
    , m_type_id(TypeID::template get<T>()) {
}

template <typename Family>
typename EventManager<Family>::TypeID EventManager<Family>::Metadata::get_type_id() const {
    return m_type_id;
}


template <typename Family>
void EventManager<Family>::register_event(const EventManager::Metadata &metadata) {
    Index type_index = metadata.get_type_id().get_index();
    if (type_index >= m_callbacks.size()) {
        m_callbacks.resize(type_index + 1);
    }
    m_callbacks.at(type_index) = std::move(metadata.create_callbacks_storage());
}

template <typename Family>
template <typename T, typename... Args>
void EventManager<Family>::process_event(Args &&... args) {
    using Event = typename Events::template Decay<T>;
    Event event = Event(std::forward<Args>(args)...);
    get_storage<Event>().handle(event);
    if constexpr (detail::HasFinalize<Event>::value) {
        event.finalize();
    }
}

template <typename Family>
template <typename T>
typename EventManager<Family>::Events::template Callbacks<T> &EventManager<Family>::get_storage() {
    Index type_index = Metadata::template get<T>().get_type_id().get_index();
    typename Events::ICallbacks *storage = m_callbacks[type_index].get();
    return fast_dynamic_cast<typename Events::template Callbacks<T> &>(*storage);
}


} // namespace secs

#endif // SECS_EVENT_MANAGER_INL
