#ifndef SECS_ENGINE_HPP
#define SECS_ENGINE_HPP

template <typename InstanceID>
class EngineInstance {
public:
    class EntityManager;
    class EventManager;
    class SystemManager;

    static EntityManager entities;
    static EventManager events;
    static SystemManager systems;

    static EngineInstance instance;

private:
    EngineInstance() = default;

};

#ifndef SECS_ENGINE_INL
#include "impl/engine.inl"
#endif

#ifndef SECS_NO_DEFAULT_ENGINE
    namespace internal {

        class DefaultEngine {
        private:
            class InstanceID {
            };

        public:
            using Type = EngineInstance<InstanceID>;

        };

    }

    using Engine = internal::DefaultEngine::Type;
#else
    template <typename InstanceID>
    using Engine = EngineInstance<InstanceID>;
#endif

#endif // SECS_ENGINE_HPP
