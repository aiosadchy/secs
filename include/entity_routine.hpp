#ifndef SECS_ENTITY_ROUTINE_HPP
#define SECS_ENTITY_ROUTINE_HPP


#include "component.hpp"

namespace EntityRoutine {

    class ComponentReference {
    public:
        inline ComponentReference();

        template <typename C>
        inline void init(C *component);

        template <typename C>
        inline C *get() const;

    private:
        void *m_component;
        Component::TypeID m_typeID;

    };

    struct Metadata {
        using EntityDestructor = void (*)(void *);

        union {
            ComponentReference *components;
            EntityDestructor destructor;
        };
        unsigned generation;
        bool alive;

        inline Metadata();

    };

    // This class is needed to do "dirty trick" with entity construction
    // that will allow inherited classes to use components in constructors
    template <typename E>
    class Holder {
    public:
        inline Holder();

        inline Metadata &metadata();
        inline E &entity();

    private:
        static constexpr std::size_t size();
        static constexpr std::size_t entityOffset();
        static constexpr std::size_t alignment();
        static constexpr std::size_t actualMetadataSize();

        alignas(alignment()) unsigned char m_data[size()];

    };

}

#endif // SECS_ENTITY_ROUTINE_HPP

#include "impl/entity_routine.inl"
