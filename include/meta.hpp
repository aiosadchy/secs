#ifndef SECS_META_HPP
#define SECS_META_HPP


#include "type_id.hpp"
#include "utility.hpp"

namespace Meta {

    class Component : public Static {
    private:
        class ComponentTypeIDFamily {};

    public:
        using TypeID = ::TypeID<ComponentTypeIDFamily>;

        class SafePtr {
        public:
            inline SafePtr();

            template <typename C>
            inline void init(C *component);

            template <typename C>
            inline C *get() const;

        private:
            void  *m_component;
            TypeID m_typeID;

        };

    };

    class Entity : public Static {
    private:
        class EntityTypeIDFamily {};

    public:
        using TypeID = ::TypeID<EntityTypeIDFamily>;

        class Controller;

        template <typename M, typename E>
        class Body;

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
        inline E &getEntity();

    private:
        static constexpr std::size_t size();
        static constexpr std::size_t align();
        static constexpr std::size_t entityOffset();

        alignas(align()) unsigned char m_data[size()];

    };

}

#endif // SECS_META_HPP

