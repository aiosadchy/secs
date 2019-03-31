#ifndef SECS_ENTITY_MANAGER_INL
#define SECS_ENTITY_MANAGER_INL


#include <utility>
#include "../component.hpp"
#include "../entity_manager.hpp"

template<typename Application>
template<typename E, typename... Args>
EntityHandle EntityManager<Application>::create(Args&& ...args) {
    static_assert(std::is_base_of<Entity, E>::value);

    // TODO: implementation

}

template<typename Application>
void EntityManager<Application>::destroy(const EntityHandle &entity) {
    if (entity.alive()) {
        entity.m_entity->destroy();
    }
}

template<typename Application>
template<typename E>
void EntityManager<Application>::destroyEntity(EntityBase *base) {
    static_assert(std::is_base_of<Entity, E>::value);

    // TODO: implementation

}

template<typename Application>
template<typename E, typename First, typename... Rest>
void EntityManager<Application>::destroyComponents(E *entity) {
    static_assert(std::is_base_of<Entity, E>::value);
    static_assert(std::is_base_of<Component, First>::value);

    // TODO: implementation

    constexpr if (sizeof...(Rest) != 0) {
        destroyComponents<E, Rest...>(entity);
    }
}

template<typename Application>
template<typename E, typename First, typename... Rest>
void EntityManager<Application>::createComponents(E *entity) {
    static_assert(std::is_base_of<Entity, E>::value);
    static_assert(std::is_base_of<Component, First>::value);

    // TODO: implementation

    constexpr if (sizeof...(Rest) != 0) {
        createComponents<E, Rest...>(entity);
    }
}

template<typename Application>
template<typename C>
void EntityManager<Application>::destroyComponent(C *component) {
    static_assert(std::is_base_of<Component, C>::value);

    // TODO: implementation
}


#endif // SECS_ENTITY_MANAGER_INL
