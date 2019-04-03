#ifndef SECS_META_HPP
#define SECS_META_HPP


#include "component.hpp"

template <typename ...Components>
class Entity;

namespace Meta {

    class Component {
    public:
        class SafePtr {
        public:
            inline SafePtr();

            template <typename C>
            inline void init(C *component);

            template <typename C>
            inline C *get() const;

        private:
            void *m_component;
            ::Component::TypeID m_typeID;

        };

    };

    class Entity {
    private:
        class EntityTypeIDFamily {};

        class Base : public Immovable {};

        template <typename ...Components>
        friend class ::Entity;

    public:
        using TypeID = ::TypeID<EntityTypeIDFamily>;

        class Controller;

        template <typename M, typename E>
        class Body;

    };

    class Entity::Controller : public Immovable {
    public:
        using Destructor = void (*)(Controller *);

        const Component::SafePtr *getComponents() const;
        TypeID getTypeID() const;
        unsigned getGeneration() const;
        bool isAlive() const;

        template <typename E, typename ...Args>
        void init(const Component::SafePtr *components, Args&& ...args);

        void destroy();

        template <typename E>
        E *getEntity() const;

    private:
        union {
            Destructor m_destructor;
            const Component::SafePtr *m_components;
        };
        unsigned m_generation;
        const TypeID m_typeID;
        bool m_alive;
        const unsigned char m_offset;

        inline Controller(Destructor destructor, TypeID typeID, unsigned char offset);
        static constexpr std::size_t actualSize();

        template <typename M, typename E>
        friend class Entity::Body;

    };

    template <typename M, typename E>
    class Entity::Body : public Immovable {
    public:
        inline Body();
        inline Controller &controller();
        inline E &entity();

    private:
        static constexpr std::size_t size();
        static constexpr std::size_t align();
        static constexpr std::size_t entityOffset();

        alignas(align()) unsigned char m_data[size()];

    };

}

#endif // SECS_META_HPP

