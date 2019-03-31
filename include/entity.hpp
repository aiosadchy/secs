#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP


#include "utility.hpp"
#include "type_set.hpp"

template <typename ...Components>
class Entity : public Immovable {
public:
    template <typename T>
    T *get();

protected:
    Entity();

private:
    TypeSet<Components*...> m_components;

};


#endif // SECS_ENTITY_HPP
