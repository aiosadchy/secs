#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP


#include "utility.hpp"

class Component : public Immovable {
public:
    template <typename ...Components>
    static constexpr bool suitableFor();

protected:
    Component() = default;

};


template <typename ...Requirements>
class DependentComponent : public Component {
public:
    template <typename ...Components>
    static constexpr bool suitableFor();

protected:
    DependentComponent() = default;

};


#endif // SECS_COMPONENT_HPP
