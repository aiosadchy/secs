#ifndef SECS_CALLBACK_STORAGE_INL
#define SECS_CALLBACK_STORAGE_INL

#include "SECS/collections/callback_storage.hpp"

namespace secs {

CallbackID::CallbackID()
    : m_guard(nullptr)
    , m_index(0) {
}

CallbackID::CallbackID(const ICallbackStorage *guard, Index index)
    : m_guard(guard)
    , m_index(index) {
}


template <typename T>
CallbackStorage<T>::CallbackStorage()
    : m_callbacks()
    , m_next_index(0)
    , m_callback_id_guard(this) {
}

template <typename T>
CallbackID CallbackStorage<T>::register_callback(Callback &&function) {
    auto &pair = m_callbacks.emplace_back(m_next_index, std::move(function));
    m_next_index++;
    return CallbackID(this, pair->first);
}

template <typename T>
void CallbackStorage<T>::unregister_callback(const CallbackID &id) {
    if (id.m_guard != m_callback_id_guard) {
        return;
    }
    auto it = std::find_if(
        m_callbacks.begin(),
        m_callbacks.end(),
        [&id](const std::pair<Index, Callback> &pair) {
            pair.first == id.m_index;
        }
    );
    if (it != m_callbacks.end()) {
        m_callbacks.erase(it);
    }
}

template <typename T>
void CallbackStorage<T>::handle(const T &event) {
    for (auto &[index, callback] : m_callbacks) {
        callback(event);
    }
}

} // namespace secs

#endif // SECS_CALLBACK_STORAGE_INL
