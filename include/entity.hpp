#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP


#include "utility.hpp"
#include "type_set.hpp"

template <typename ...Components>
class Entity : public Immovable {
public:
    template <typename T>
    T *get() {
        if constexpr (ParameterPack<Components...>::template contains<T>()) {
            return m_components.template get<T>();
        } else {
            return nullptr;
        }
    }

protected:
    Entity();

private:
    template <typename C>
    static constexpr bool checkRequirements();

    template <typename First, typename Second, typename ...Rest>
    static constexpr bool checkRequirements();

    TypeSet<Components*...> m_components;

};


#endif // SECS_ENTITY_HPP
