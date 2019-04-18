#ifndef SECS_META_INL
#define SECS_META_INL


#include "../component.hpp"
#include "../entity.hpp"
#include "../meta.hpp"

namespace Meta {

    inline const Component::SafePtr *Entity::Controller::getComponents() const {
        if (m_state == State::IN_CONSTRUCTION) {
            return m_components;
        } else {
            return nullptr;
        }
    }

    template <typename E>
    inline bool Entity::Controller::isOfType() const {
        static_assert(std::is_base_of<::Entity, E>::value);
        return m_typeID == TypeID::get<E>();
    }

    inline unsigned Entity::Controller::getGeneration() const {
        return m_generation;
    }

    inline bool Entity::Controller::isAlive() const {
        return (m_state == State::ALIVE);
    }

    template <typename E>
    inline E *Entity::Controller::getEntity() const {
        if (isOfType<E>()) {
            unsigned char *raw = reinterpret_cast<unsigned char *>(const_cast<Controller *>(this));
            return reinterpret_cast<E *>(raw + m_offset);
        } else {
            return nullptr;
        }
    }

    template <typename E, typename... Args>
    inline void Entity::Controller::init(const Component::SafePtr *components, Args&& ...args) {
        if ((m_state == State::DEAD) && isOfType<E>()) {
            Destructor destructor = m_destructor;
            m_components = components;
            m_state = State::IN_CONSTRUCTION;
            new (getEntity<E>()) E(std::forward<Args>(args)...);
            m_state = State::ALIVE;
            m_destructor = destructor;
        }
    }

    inline void Entity::Controller::destroy() {
        if (isAlive()) {
            m_destructor(this);
            m_state = State::DEAD;
            m_generation++;
        }
    }

    inline Entity::Controller::Controller(Destructor destructor, TypeID typeID, unsigned char offset) :
            m_destructor(destructor),
            m_generation(1),
            m_typeID(typeID),
            m_state(State::DEAD),
            m_offset(offset) {}

    inline constexpr std::size_t Entity::Controller::actualSize() {
        static_assert(std::is_standard_layout<Controller>::value);
        std::size_t result = offsetof(Controller, m_components) + sizeof(m_components);
        result = max(result, offsetof(Controller, m_destructor) + sizeof(m_destructor));
        result = max(result, offsetof(Controller, m_typeID) + sizeof(m_typeID));
        result = max(result, offsetof(Controller, m_generation) + sizeof(m_generation));
        result = max(result, offsetof(Controller, m_state) + sizeof(m_state));
        return result;
    }


    template <typename M, typename E>
    inline Entity::Body<M, E>::Body() : m_data() {
        static_assert(std::is_base_of<::Entity, E>::value);
        static_assert(entityOffset() <= static_cast<std::size_t>(std::numeric_limits<unsigned char>::max()));
        unsigned char offset = static_cast<unsigned char>(entityOffset());
        m_data[entityOffset() - 1] = offset;
        new (&getController()) Controller(M::template destroy<E>, TypeID::get<E>(), offset);
    }

    template <typename M, typename E>
    inline Entity::Controller &Entity::Body<M, E>::getController() {
        return *reinterpret_cast<Controller *>(m_data);
    }

    template <typename M, typename E>
    inline E *Entity::Body<M, E>::getEntity() {
        return reinterpret_cast<E *>(m_data + entityOffset());
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


    inline Entity::Reference::Reference(const Entity::Controller *c) :
        m_controller(c),
        m_generation((c != nullptr) ? c->getGeneration() : 0) {}

    inline bool Entity::Reference::isAlive() const {
        return (m_controller != nullptr)
            && (m_controller->getGeneration() == m_generation)
            && (m_controller->isAlive());
    }

    inline Entity::Reference::operator bool() const {
        return isAlive();
    }

    inline void Entity::Reference::invalidate() {
        m_controller = nullptr;
        m_generation = 0;
    }


    template <typename E>
    inline Entity::Handle<E>::Handle(const Entity::Controller *c) : Reference(c), m_entity(nullptr) {
        static_assert(std::is_base_of<::Entity, E>::value);
        if ((c != nullptr) && (c->isOfType<E>())) {
            m_entity = c->getEntity<E>();
        } else {
            invalidate();
        }
    }

    template <typename E>
    inline E *Entity::Handle<E>::operator->() {
        return getEntity();
    }

    template <typename E>
    inline const E *Entity::Handle<E>::operator->() const {
        return getEntity();
    }

    template <typename E>
    inline E &Entity::Handle<E>::operator*() {
        return *getEntity();
    }

    template <typename E>
    inline const E &Entity::Handle<E>::operator*() const {
        return *getEntity();
    }

    template <typename E>
    inline E *Entity::Handle<E>::getEntity() {
        return isAlive() ? m_entity : nullptr;
    }

    template <typename E>
    inline const E *Entity::Handle<E>::getEntity() const {
        return const_cast<Handle<E> *>(this)->getEntity();
    }



    template <typename C>
    inline bool Component::Controller<C>::isAlive() const {
        return m_entity.isAlive();
    }

    template <typename C>
    inline Entity::Reference &Component::Controller<C>::getEntity() const {
        return m_entity;
    }

    template <typename C>
    inline C &Component::Controller<C>::getComponent() const {
        return *reinterpret_cast<C *>(m_component);
    }

    template <typename C>
    template <typename... Args>
    inline void Component::Controller<C>::init(const Entity::Reference &entity, Args&& ...args) {
        m_entity = entity;
        new (&getComponent()) C(std::forward<Args>(args)...);
    }

    template <typename C>
    inline void Component::Controller<C>::destroy() {
        m_entity.invalidate();
        getComponent().~C();
    }


    template <typename C>
    inline Component::Handle<C>::Handle(const Component::Controller<C> *c) : m_controller(c) {}

    template <typename C>
    inline bool Component::Handle<C>::isAlive() const {
        return (m_controller != nullptr) && (m_controller->isAlive());
    }

    template <typename C>
    inline Component::Handle<C>::operator bool() const {
        return isAlive();
    }

    template <typename C>
    inline C *Component::Handle<C>::operator->() {
        return getComponent();
    }

    template <typename C>
    inline const C *Component::Handle<C>::operator->() const {
        return getComponent();
    }

    template <typename C>
    inline C &Component::Handle<C>::operator*() {
        return *getComponent();
    }

    template <typename C>
    inline const C &Component::Handle<C>::operator*() const {
        return *getComponent();
    }

    template <typename C>
    inline C *Component::Handle<C>::getComponent() {
        return isAlive() ? &m_controller->getComponent() : nullptr;
    }

    template <typename C>
    inline const C *Component::Handle<C>::getComponent() const {
        return const_cast<Handle<C> *>(this)->getComponent();
    }

    template <typename C>
    inline Entity::Reference *Component::Handle<C>::getEntity() {
        return isAlive() ? &m_controller->getEntity() : nullptr;
    }

    template <typename C>
    inline const Entity::Reference *Component::Handle<C>::getEntity() const {
        return const_cast<Handle<C> *>(this)->getEntity();
    }


    inline Component::SafePtr::SafePtr() : m_component(nullptr), m_typeID(TypeID::INVALID) {}

    template <typename C>
    inline void Component::SafePtr::init(Component::Controller<C> *component) {
        static_assert(std::is_base_of<::Component, C>::value);
        if (component != nullptr) {
            m_typeID = TypeID::get<C>();
        } else {
            m_typeID = TypeID::INVALID;
        }
        m_component = component;
    }

    template <typename C>
    inline Component::Controller<C> *Component::SafePtr::get() const {
        static_assert(std::is_base_of<::Component, C>::value);
        return  (m_typeID == TypeID::get<C>()) ? m_component : nullptr;
    }

}

#endif // SECS_META_INL
