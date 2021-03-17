#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP

#include <memory>
#include <type_traits>

#include <utl/non_constructable.hpp>
#include <utl/type_id.hpp>
#include <utl/type_info.hpp>

#include "SECS/collections/component_pool.hpp"
#include "SECS/common.hpp"


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

    using TypeID = utl::TypeID<Component<Family>, Index, Decay, true>;

    class Metadata : public utl::TypeInfo<Metadata, Decay, false> {
    public:
        inline PoolHandle create_pool(Index capacity) const;
        inline TypeID get_type_id() const;

        static const Metadata *first();
        static const Metadata *next(const Metadata *record);

    private:
        friend class utl::TypeInfo<Metadata, Decay, false>;
        using Base = utl::TypeInfo<Metadata, Decay, false>;
        using CreatePool = PoolHandle (Index);

        template <typename T>
        explicit Metadata(typename Base::template Initializer<T>);

        CreatePool * const m_create_pool;
        const TypeID m_type_id;
        const Metadata * const m_next;

        inline static const Metadata *s_head = nullptr;

    };

    class Iterator {
    public:
        Iterator();
        explicit Iterator(const Metadata *item);
        const Metadata &operator*() const;
        const Iterator &operator++();
        bool operator!=(const Iterator &another) const;
    private:
        const Metadata *m_item;

    };

    class View {
    public:
        Iterator begin() const;
        Iterator end() const;
    };

    static View iterate();

    NON_CONSTRUCTABLE(Component)

};

#endif // SECS_COMPONENT_HPP
