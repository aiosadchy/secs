#ifndef SECS_COMPONENT_INL
#define SECS_COMPONENT_INL


#include "../component.hpp"

template <typename ...Requirements>
template <typename T>
T &Component::Dependent<Requirements...>::get() {
    static_assert(Dependencies::template contains<T>(), "Requested type is not a dependency of component");
    return *m_deps.template get<T *>();
}

template <typename ...Requirements>
template <typename T>
const T &Component::Dependent<Requirements...>::get() const {
    return const_cast<const Dependent<Requirements...> *>(this)->template get<T>();
}


#endif // SECS_COMPONENT_INL
