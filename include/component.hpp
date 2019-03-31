#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP


#include "parameter_pack.hpp"
#include "utility.hpp"
#include "type_set.hpp"

class Component : public Immovable {
public:
    using Dependencies = PPack<>;

    template <typename ...Requirements>
    class Dependent;

protected:
    Component() = default;
    ~Component() = default;

};

template <typename ...Requirements>
class Component::Dependent : public Component {
public:
    using Dependencies = PPack<Requirements...>;

    template <typename T>
    T &get();

    template <typename T>
    const T &get() const;

protected:
    Dependent() = default;
    ~Dependent() = default;

private:
    TypeSet<typename PackOf<Dependencies>::Pointers> m_deps;

};


#endif // SECS_COMPONENT_HPP
