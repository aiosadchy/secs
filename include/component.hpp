#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP


class Component {
public:
    template <typename... Components>
    static constexpr bool metRequirements();

protected:
    Component() = default;

};


template <typename... Requirements>
class DependentComponent : public Component {
public:
    template <typename... Components>
    static constexpr bool metRequirements();

protected:
    DependentComponent() = default;

};


#endif // SECS_COMPONENT_HPP
