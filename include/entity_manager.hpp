#ifndef SECS_ENTITY_MANAGER_HPP
#define SECS_ENTITY_MANAGER_HPP


#include "entity.hpp"
#include "event_manager.hpp"

template <typename Application>
class EntityManager {
public:
    using Events = EventManager<Application>;

    // template <typename E, typename ...Args>
    // EntityHandle create(Args&& ...args);

    // void destroy(const EntityHandle &entity);

private:
    // template <typename E>
    // void destroyEntity(EntityBase *entity);

    template <typename E, typename First, typename ...Rest>
    void destroyComponents(E *entity);

    template <typename E, typename First, typename ...Rest>
    void createComponents(E *entity);

    template <typename C>
    void destroyComponent(C *component);

};


#endif // SECS_ENTITY_MANAGER_HPP
