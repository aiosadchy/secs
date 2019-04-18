#ifndef SECS_COMPONENT_INL
#define SECS_COMPONENT_INL


#include "../component.hpp"

template <typename ...D>
template <typename T>
T &Component::Dependent<D...>::get() {
    static_assert(Dependencies::template contains<T>(), "Requested type is not a dependency of component");
    return *m_deps.template get<T *>();
}

template <typename ...D>
template <typename T>
const T &Component::Dependent<D...>::get() const {
    return const_cast<Dependent<D...> *>(this)->template get<T>();
}


#endif // SECS_COMPONENT_INL
