#include "component.hpp"
#include "utility.hpp"

template<typename ...Components>
constexpr bool Component::suitableFor() {
    return true;
}

template<typename ...Requirements>
template<typename ...Components>
constexpr bool DependentComponent<Requirements...>::suitableFor() {
    return ParameterPack<Components...>::template contains<Requirements...>();
}