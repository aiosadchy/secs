#ifndef SECS_META_HPP
#define SECS_META_HPP


#include "type_id.hpp"
#include "utility.hpp"

namespace Meta {

    class Entity : public Static {
    private:
        class EntityTypeIDFamily {};

    public:
        using TypeID = ::TypeID<EntityTypeIDFamily>;

        class Controller;

        template <typename M, typename E>
        class Body;

        class Reference;

        template <typename E>
        class Handle;

    };


    class Component : public Static {
    private:
        class ComponentTypeIDFamily {};

    public:
        using TypeID = ::TypeID<ComponentTypeIDFamily>;

        template <typename C>
        class Controller;

        template <typename C>
        class Handle;

        class SafePtr;

    };


    class Entity::Controller : public Immovable {
    public:
        using Destructor = void (*)(Controller *);

        inline const Component::SafePtr *getComponents() const;

        template <typename E>
        inline bool isOfType() const;

        inline unsigned getGeneration() const;

        inline bool isAlive() const;

        template <typename E>
        inline E *getEntity() const;

        template <typename E, typename ...Args>
        inline void init(const Component::SafePtr *components, Args&& ...args);

        inline void destroy();

    private:
        template <typename M, typename E>
        friend class Entity::Body;

        enum class State : unsigned char {
            DEAD,
            IN_CONSTRUCTION,
            ALIVE
        };

        union {
            Destructor m_destructor;
            const Component::SafePtr *m_components;
        };

        unsigned            m_generation;
        const TypeID        m_typeID;
        State               m_state;
        const unsigned char m_offset;

        inline Controller(Destructor destructor, TypeID typeID, unsigned char offset);
        inline static constexpr std::size_t actualSize();

    };


    template <typename M, typename E>
    class Entity::Body : public Immovable {
    public:
        inline Body();
        inline Controller &getController();
        inline E *getEntity();

    private:
        static constexpr std::size_t size();
        static constexpr std::size_t align();
        static constexpr std::size_t entityOffset();

        alignas(align()) unsigned char m_data[size()];

    };


    class Entity::Reference {
    public:
        inline explicit Reference(const Controller *c = nullptr);
        inline bool isAlive() const;
        inline operator bool() const;
        inline void invalidate();

    private:
        const Entity::Controller *m_controller;
        unsigned m_generation;

    };


    template <typename E>
    class Entity::Handle : public Entity::Reference {
    public:
        inline explicit Handle(const Controller *c = nullptr);

        inline E *operator->();
        inline const E *operator->() const;

        inline E &operator*();
        inline const E &operator*() const;

        inline E *getEntity();
        inline const E *getEntity() const;

    private:
        E *m_entity;

    };


    template <typename C>
    class Component::Controller {
    public:
        inline bool isAlive() const;

        inline Entity::Reference &getEntity() const;
        inline C &getComponent() const;

        template <typename ...Args>
        inline void init(const Entity::Reference &entity, Args&& ...args);

        inline void destroy();

    private:
        Entity::Reference m_entity;
        alignas(C) unsigned char m_component[sizeof(C)];

    };


    template <typename C>
    class Component::Handle {
    public:
        inline explicit Handle(const Controller<C> *c = nullptr);
        inline bool isAlive() const;
        inline operator bool() const;

        inline C *operator->();
        inline const C *operator->() const;

        inline C &operator*();
        inline const C &operator*() const;

        inline C *getComponent();
        inline const C *getComponent() const;

        inline Entity::Reference *getEntity();
        inline const Entity::Reference *getEntity() const;

    private:
        Component::Controller<C> *m_controller;

    };


    class Component::SafePtr {
    public:
        inline SafePtr();

        template <typename C>
        inline void init(Controller<C> *component);

        template <typename C>
        inline Controller<C> *get() const;

    private:
        void  *m_component;
        TypeID m_typeID;

    };

}

#endif // SECS_META_HPP

