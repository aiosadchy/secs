#ifndef SECS_COMPONENT_INL
#define SECS_COMPONENT_INL


#include "../component.hpp"

template<typename... Requirements>
template<typename T>
T &Component::Dependent<Requirements...>::get() {
    return *m_deps.template get<T>();
}

template<typename... Requirements>
template<typename T>
const T &Component::Dependent<Requirements...>::get() const {
    return *m_deps.template get<T>();
}


#endif // SECS_COMPONENT_INL
