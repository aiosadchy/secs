#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP

#include <type_traits>

#include <utl/non_constructible.hpp>
#include <utl/type_id.hpp>
#include <utl/type_info.hpp>

#include "SECS/collections/component_pool.hpp"
#include "SECS/collections/linked_metadata.hpp"
#include "SECS/common.hpp"


namespace secs {

template <typename Family>
class Component {
public:
    template <typename C>
    using Decay = std::decay_t<C>;

    using IPool = IComponentPool;

    template <typename C>
    using Pool = ComponentPool<Decay<C>>;

    using TypeID = utl::TypeID<Component<Family>, Index, Decay, utl::init::TypeID::LAZY>;

    UTL_NON_CONSTRUCTIBLE(Component)

};

} // namespace secs

#endif // SECS_COMPONENT_HPP
