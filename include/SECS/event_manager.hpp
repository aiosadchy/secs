#ifndef SECS_EVENT_MANAGER_HPP
#define SECS_EVENT_MANAGER_HPP

#include <deque>
#include <memory>
#include <queue>
#include <utility>
#include <vector>

#include <utl/non_copyable.hpp>

#include "SECS/event.hpp"


namespace secs {

template <typename Family>
class EventManager {
public:
    using Events = Event<Family>;

    inline EventManager();
    inline ~EventManager();

    UTL_NON_COPYABLE(EventManager)

    template <typename E>
    void handle(E &&event);

    template <typename E, typename... Args>
    void handle(Args &&... args);

    template <typename F>
    CallbackID register_callback(F &&function);

    template <typename... E>
    void register_event();

private:
    using CallbacksHandle = std::unique_ptr<typename Events::ICallbacks>;

    using TypeID = typename Events::TypeID;

    template <typename T>
    using Decay = typename Events::template Decay<T>;

    class Metadata : public LinkedMetadata<Metadata, Decay> {
    public:
        using CreateStorage = CallbacksHandle (*)();

        template <typename T>
        explicit Metadata(typename Metadata::template Initializer<T>);

        const CreateStorage create_callbacks_storage;
        inline TypeID get_type_id() const;

    private:
        const TypeID m_type_id;

    };

    void register_event(const Metadata &metadata);

    template <typename T, typename... Args>
    void process_event(Args &&... args);

    template <typename T>
    typename Events::template Callbacks<T> &get_storage();

    std::vector<CallbacksHandle> m_callbacks;

    // TODO: thread_local event queues

};

} // namespace secs

#endif // SECS_EVENT_MANAGER_HPP
