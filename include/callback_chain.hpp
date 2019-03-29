#ifndef SECS_CALLBACK_CHAIN_HPP
#define SECS_CALLBACK_CHAIN_HPP


#include <functional>
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

    CallbackID(CallbackChain<E> &chain, Key key);

};

template <typename E>
class CallbackChain : public Immovable {
public:
    using Function = std::function<void(const E &)>;
    using ID = CallbackID<E>;

    ID add(const Function &function);
    bool remove(const ID &id);
    void fire(const E &event);

    CallbackChain();

private:
    std::vector<std::pair<typename ID::Key, Function>> m_callbacks;
    typename ID::Key m_nextKey;

};


#endif // SECS_CALLBACK_CHAIN_HPP
