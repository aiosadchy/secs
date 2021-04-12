#ifndef SECS_CALLBACK_STORAGE_HPP
#define SECS_CALLBACK_STORAGE_HPP

#include <functional>
#include <vector>

#include "SECS/common.hpp"


namespace secs {

class CallbackID;

class ICallbackStorage {
public:
    virtual ~ICallbackStorage() = default;
    virtual void unregister_callback(const CallbackID &id) = 0;

};

class CallbackID {
public:
    CallbackID();

private:
    template <typename T>
    friend class CallbackStorage;

    CallbackID(const ICallbackStorage *guard, Index index);

    const ICallbackStorage *m_guard;
    Index m_index;

};

template <typename T>
class CallbackStorage : public ICallbackStorage {
public:
    using Callback = std::function<void(const T &)>;

    CallbackStorage();
    ~CallbackStorage() override = default;

    CallbackID register_callback(Callback &&function);
    void unregister_callback(const CallbackID &id) override;
    void handle(const T &event);

private:
    std::vector<std::pair<Index, Callback>> m_callbacks;
    Index m_next_index;
    ICallbackStorage *m_callback_id_guard;

};

} // namespace secs

#endif // SECS_CALLBACK_STORAGE_HPP
