#ifndef SECS_COMPONENT_HPP
#define SECS_COMPONENT_HPP

#include <type_traits>
#include <utl/type_info.hpp>
#include "SECS/collections/component_pool.hpp"
#include "SECS/common.hpp"


template <typename Family>
class Component {
public:
    template <typename C>
    using RawType = std::decay_t<C>;

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

    class Metadata : public utl::TypeInfo<Component<Family>::Metadata, false, RawType> {
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
            Iterator begin() const;
            Iterator end() const;
        };

        template <typename T>
        explicit Metadata(utl::Type<T>);

        inline AbstractComponentPool *create_pool(Index initial_capacity) const;
        inline TypeID get_type_id() const;
        static Index get_registered_types_count();

        static const List &iterable();

    private:
        using PoolFactory = AbstractComponentPool *(*)(Index);

        friend class Iterator;
        friend class List;

        PoolFactory m_pool_factory;
        Index m_type_index;
        Metadata *m_next;

        static const Metadata *s_head = nullptr;

    };

};

#endif // SECS_COMPONENT_HPP

#include "impl/component.inl"
