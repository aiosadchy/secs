#include "component.hpp"
#include "utility.hpp"

template<typename... Components>
constexpr bool Component::metRequirements() {
    return true;
}

template<typename... Requirements>
template<typename... Components>
constexpr bool DependentComponent<Requirements...>::metRequirements() {
    return ParameterPack<Components...>::template contains<Requirements...>();
}