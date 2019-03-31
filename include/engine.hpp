#ifndef SECS_ENGINE_HPP
#define SECS_ENGINE_HPP


#include "entity_manager.hpp"
#include "event_manager.hpp"

template <typename Application>
class Engine {
public:
    using Entities = EntityManager<Application>;
    using Events = EventManager<Application>;
};


#endif // SECS_ENGINE_HPP
