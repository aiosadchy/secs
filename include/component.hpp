#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP


#include "parameter_pack.hpp"
#include "utility.hpp"

class Component : public Immovable {
public:
    using Dependencies = PPack<>;

protected:
    Component() = default;

};


template <typename ...Requirements>
class DependentComponent : public Component {
public:
    using Dependencies = PPack<Requirements...>;

protected:
    DependentComponent() = default;

};


#endif // SECS_COMPONENT_HPP
