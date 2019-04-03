#ifndef SECS_ENTITY_ROUTINE_INL
#define SECS_ENTITY_ROUTINE_INL


#include <limits>
#include "../entity_routine.hpp"

namespace EntityRoutine {

    inline ComponentReference::ComponentReference() : m_component(nullptr), m_typeID(Component::TypeID::INVALID) {}

    template <typename C>
    inline void ComponentReference::init(C *component) {
        m_typeID = (component != nullptr) ? Component::TypeID::get<C>() : Component::TypeID::INVALID;
        m_component = component;
    }

    template <typename C>
    inline C *ComponentReference::get() const {
        return (Component::TypeID::get<C>() == m_typeID) ? m_component : nullptr;
    }

    inline Metadata::Metadata() : destructor(nullptr), generation(1), alive(false) {}

    template <typename E>
    inline Holder<E>::Holder() {
        static_assert(entityOffset() <= std::numeric_limits<unsigned char>::max());
        new (&metadata()) Metadata();
        m_data[entityOffset() - 1] = static_cast<unsigned char>(entityOffset());
    }

    template <typename E>
    inline Metadata &Holder<E>::metadata() {
        return *reinterpret_cast<Metadata *>(m_data);
    }

    template <typename E>
    inline E &Holder<E>::entity() {
        return *reinterpret_cast<E *>(m_data + entityOffset());
    }

    template <typename E>
    constexpr std::size_t Holder<E>::size() {
        return entityOffset() + sizeof(E);
    }

    template <typename E>
    constexpr std::size_t Holder<E>::entityOffset() {
        std::size_t metasize = actualMetadataSize() + 1;
        std::size_t offset = (metasize / alignment()) + ((metasize % alignment()) > 0 ? 1 : 0);
        return offset * alignment();
    }

    template <typename E>
    constexpr std::size_t Holder<E>::alignment() {
        std::size_t align = (alignof(Metadata) > alignof(E)) ? alignof(Metadata) : alignof(E);
        return (align > 0) ? align : 1;
    }

    template <typename E>
    constexpr std::size_t Holder<E>::actualMetadataSize() {
        std::size_t result = offsetof(Metadata, components) + sizeof(Metadata::components);
        std::size_t candidate = offsetof(Metadata, destructor) + sizeof(Metadata::destructor);
        if (candidate > result) {
            result = candidate;
        }
        candidate = offsetof(Metadata, generation) + sizeof(Metadata::generation);
        if (candidate > result) {
            result = candidate;
        }
        return result;
    }

}

#endif // SECS_ENTITY_ROUTINE_INL
