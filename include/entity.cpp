#include "entity.hpp"

template<typename ...Components>
Entity<Components...>::Entity() {
    static_assert(checkRequirements<Components...>(), "Entity does not have enough components");
}

template<typename ...Components>
template<typename C>
constexpr bool Entity<Components...>::checkRequirements() {
    static_assert(C::template suitableFor<Components...>(), "Entity: component requirements are not satisfied");
    return C::template suitableFor<Components...>();
}

template<typename ...Components>
template<typename First, typename Second, typename ...Rest>
constexpr bool Entity<Components...>::checkRequirements() {
    return checkRequirements<First>() && checkRequirements<Second, Rest...>();
}


