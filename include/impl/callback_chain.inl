#ifndef SECS_CALLBACK_CHAIN_INL
#define SECS_CALLBACK_CHAIN_INL


#include "../callback_chain.hpp"

template <typename E>
inline CallbackID<E>::CallbackID(CallbackChain<E> &chain, Key key) : m_holder(&chain), m_key(key) {}

template <typename E>
inline typename CallbackChain<E>::ID CallbackChain<E>::add(const CallbackChain::Function &function) {
    ID result = CallbackChain::ID(*this, m_nextKey++);
    m_callbacks.emplace_back(result.m_key, function);
    return result;
}

template <typename E>
inline bool CallbackChain<E>::remove(const ID &id) {
    if (id.m_holder != this) {
        return false;
    }
    for (auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it) {
        if (it->first == id.m_key) {
            m_callbacks.erase(it);
            return true;
        }
    }
    return false;
}

template <typename E>
inline void CallbackChain<E>::fire(const E &event) {
    for (auto &p : m_callbacks) {
        p.second(event);
    }
}

template <typename E>
inline CallbackChain<E>::CallbackChain() : m_callbacks(), m_nextKey(0) {}


#endif // SECS_CALLBACK_CHAIN_INL
