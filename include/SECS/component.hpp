#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP

#include <memory>
#include <type_traits>

#include <utl/non_constructible.hpp>
#include <utl/type_id.hpp>
#include <utl/type_info.hpp>
#include <SECS/collections/linked_metadata.hpp>

#include "SECS/collections/component_pool.hpp"
#include "SECS/common.hpp"


namespace secs {

template <typename Family>
class Component {
private:
    template <typename C>
    using Decay = std::decay_t<C>;

public:
    using IPool = IComponentPool;

    template <typename C>
    using Pool = ComponentPool<Decay<C>>;

    using PoolHandle = std::unique_ptr<IPool>;

    using TypeID = utl::TypeID<Component<Family>, Index, Decay, utl::init::TypeID::LAZY>;

    class Metadata : public LinkedMetadata<Metadata, Decay> {
    public:
        inline PoolHandle create_pool(Index capacity) const;
        inline TypeID get_type_id() const;

    private:
        using Base = LinkedMetadata<Metadata, Decay>;
        friend class Base::Base;
        using CreatePool = PoolHandle (Index);

        template <typename T>
        explicit Metadata(typename Metadata::template Initializer<T>);

        CreatePool * const m_create_pool;
        const TypeID m_type_id;

    };

    static typename Metadata::View view();

    NON_CONSTRUCTIBLE(Component)

};

} // namespace secs

#endif // SECS_COMPONENT_HPP
