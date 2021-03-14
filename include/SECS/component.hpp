#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP

#include <type_traits>

#include <utl/type_info.hpp>
#include <utl/non_constructable.hpp>

#include "SECS/collections/component_pool.hpp"
#include "SECS/common.hpp"


template <typename Family>
class Component {
private:
    template <typename C>
    using Decay = std::decay_t<C>;

public:
    using AbstractPool = AbstractComponentPool;

    template <typename C>
    using Pool = ComponentPool<Decay<C>>;

    class Metadata;

    class TypeID {
    public:
        TypeID();
        Index get_index() const;
        bool operator==(const TypeID &another) const;
        bool operator!=(const TypeID &another) const;

        template <typename T>
        static TypeID get();

    private:
        friend class Metadata;

        explicit TypeID(Index index);

        Index m_index;

    };

    class Metadata : public utl::TypeInfo<Metadata, false, Decay> {
    private:
        using Base = utl::TypeInfo<Metadata, false, Decay>;

    public:
        class Iterator {
        public:
            Iterator();
            explicit Iterator(const Metadata *item);
            const Metadata *operator->() const;
            const Metadata &operator*() const;
            const Iterator &operator++();
            bool operator!=(const Iterator &another) const;
        private:
            const Metadata *m_item;

        };

        class List {
        public:
            Iterator begin() const;
            Iterator end() const;
        };

        template <typename T>
        explicit Metadata(typename Base::template Initializer<T>);

        inline AbstractPool *create_pool(Index capacity) const;
        inline TypeID get_type_id() const;
        static Index get_registered_types_count();

        static const List &iterate();

    private:
        using PoolFactory = AbstractPool *(*)(Index);

        friend class Iterator;
        friend class List;

        PoolFactory m_pool_factory;
        Index m_type_index;
        const Metadata *m_next;

        inline static const Metadata *s_head = nullptr;

    };

    UTL_NON_CONSTRUCTABLE(Component)

};

#endif // SECS_COMPONENT_HPP
