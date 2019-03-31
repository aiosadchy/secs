#include "entity.hpp"

template<typename ...Components>
Entity<Components...>::Entity() {

}

template<typename... Components>
template<typename T>
T *Entity<Components...>::get() {
    if constexpr (PPack<Components...>::template contains<T>()) {
        return m_components.template get<T>();
    } else {
        return nullptr;
    }
}


