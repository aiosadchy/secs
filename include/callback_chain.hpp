#ifndef SECS_CALLBACK_CHAIN_HPP
#define SECS_CALLBACK_CHAIN_HPP


#include <functional>
#include <vector>
#include "utility.hpp"

template <typename E>
class CallbackChain;

template <typename E>
class CallbackID {
private:
    using Key = unsigned;

    friend class CallbackChain<E>;

    CallbackChain<E> *m_holder;
    Key m_key;

    inline CallbackID(CallbackChain<E> &chain, Key key);

};

template <typename E>
class CallbackChain : public Immovable {
public:
    using Function = std::function<void(const E &)>;
    using ID = CallbackID<E>;

    inline ID add(const Function &function);
    inline bool remove(const ID &id);
    inline void fire(const E &event);

    inline CallbackChain();

private:
    std::vector<std::pair<typename ID::Key, Function>> m_callbacks;
    typename ID::Key m_nextKey;

};


#endif // SECS_CALLBACK_CHAIN_HPP

#include "impl/callback_chain.inl"
