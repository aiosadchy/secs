#ifndef SECS_META_INL
#define SECS_META_INL


#include "../meta.hpp"

namespace Meta {

    inline Component::SafePtr::SafePtr() : m_component(nullptr), m_typeID(::Component::TypeID::INVALID) {}

    template <typename C>
    inline void Component::SafePtr::init(C *component) {
        static_assert(std::is_base_of<::Component, C>::value);
        if (component != nullptr) {
            m_typeID = ::Component::TypeID::get<C>();
        } else {
            m_typeID = ::Component::TypeID::INVALID;
        }
        m_component = component;
    }

    template <typename C>
    inline C *Component::SafePtr::get() const {
        static_assert(std::is_base_of<::Component, C>::value);
        if (m_typeID == ::Component::TypeID::get<C>()) {
            return m_component;
        } else {
            return nullptr;
        }
    }



    inline const Component::SafePtr *Entity::Controller::getComponents() const {
        if (m_alive) {
            return nullptr;
        } else {
            return m_components;
        }
    }

    inline Entity::TypeID Entity::Controller::getTypeID() const {
        return m_typeID;
    }

    inline unsigned Entity::Controller::getGeneration() const {
        return m_generation;
    }

    inline bool Entity::Controller::isAlive() const {
        return m_alive;
    }

    template <typename E, typename... Args>
    inline void Entity::Controller::init(const Component::SafePtr *components, Args &&... args) {
        static_assert(std::is_base_of<Entity::Base, E>::value);
        if (!m_alive) {
            Destructor destructor = m_destructor;
            m_components = components;
            new (getEntity<E>()) E(std::forward<Args>(args)...);
            m_destructor = destructor;
            m_alive = true;
        }
    }

    inline void Entity::Controller::destroy() {
        if (m_alive) {
            m_destructor(this);
            m_generation++;
            m_alive = false;
        }
    }

    template <typename E>
    inline E *Entity::Controller::getEntity() const {
        if (m_typeID == TypeID::get<E>()) {
            unsigned char *raw = reinterpret_cast<unsigned char *>(const_cast<Controller *>(this));
            return reinterpret_cast<E *>(raw + m_offset);
        } else {
            return nullptr;
        }
    }

    inline Entity::Controller::Controller(Destructor destructor, TypeID typeID, unsigned char offset) :
            m_destructor(destructor),
            m_generation(1),
            m_typeID(typeID),
            m_alive(false),
            m_offset(offset) {}

    inline constexpr std::size_t Entity::Controller::actualSize() {
        static_assert(std::is_standard_layout<Controller>::value);
        std::size_t result = offsetof(Controller, m_components) + sizeof(m_components);
        result = max(result, offsetof(Controller, m_destructor) + sizeof(m_destructor));
        result = max(result, offsetof(Controller, m_typeID) + sizeof(m_typeID));
        result = max(result, offsetof(Controller, m_generation) + sizeof(m_generation));
        result = max(result, offsetof(Controller, m_alive) + sizeof(m_alive));
        return result;
    }



    template <typename M, typename E>
    inline Entity::Body<M, E>::Body() : m_data() {
        static_assert(std::is_base_of<Entity::Base, E>::value);
        static_assert(entityOffset() <= static_cast<std::size_t>(std::numeric_limits<unsigned char>::max()));
        unsigned char offset = static_cast<unsigned char>(entityOffset());
        m_data[entityOffset() - 1] = offset;
        new (&controller()) Controller(M::template destroy<E>, TypeID::get<E>(), offset);
    }

    template <typename M, typename E>
    inline Entity::Controller &Entity::Body<M, E>::controller() {
        return *reinterpret_cast<Controller *>(m_data);
    }

    template <typename M, typename E>
    inline E &Entity::Body<M, E>::entity() {
        return *reinterpret_cast<E *>(m_data + entityOffset());
    }

    template <typename M, typename E>
    constexpr std::size_t Entity::Body<M, E>::size() {
        return entityOffset() + sizeof(E);
    }

    template <typename M, typename E>
    constexpr std::size_t Entity::Body<M, E>::align() {
        return max(alignof(Controller), alignof(E));
    }

    template <typename M, typename E>
    constexpr std::size_t Entity::Body<M, E>::entityOffset() {
        std::size_t header = Controller::actualSize() + 1;
        std::size_t aligns = (header / align()) + (((header % align()) > 0) ? 1 : 0);
        return aligns * align();
    }

}

#endif // SECS_META_INL
