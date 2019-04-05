#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP


#include "component.hpp"
#include "meta.hpp"
#include "type_set.hpp"
#include "utility.hpp"

class Entity : public Immovable {
private:
    template <typename ...>
    struct CDepResolver {
        using Result = PPack<>;
    };

    template <typename ...N, typename ...P, typename Comp, typename ...U>
    struct CDepResolver<PPack<N...>, PPack<P...>, PPack<Comp, U...>> {
    private:
        using CDep = typename Comp::Dependencies;

        using Need = typename PPack<N...>::
                template AddUnique<CDep, Comp>::Result;

        using Processed = typename PPack<P...>::
                template AddUnique<Comp>::Result;

        using Unprocessed = typename PPack<U...>::
                template AddUnique<CDep>::Result::
                template Subtract<Processed>::Result;

        using Next = typename CDepResolver<Need, Processed, Unprocessed>::Result;

    public:
        using Result = typename Need::template AddUnique<Next>::Result;

    };

public:
    using TypeID = Meta::Entity::TypeID;

    class Reference;

    template <typename E>
    class Handle;

    template <typename ...Components>
    class With;

protected:
    Entity() = default;
    ~Entity() = default;

};


class Entity::Reference {
public:
    inline explicit Reference(const Meta::Entity::Controller *c = nullptr);
    inline bool isValid() const;
    inline operator bool() const;
    inline void invalidate();

private:
    const Meta::Entity::Controller *m_controller;
    unsigned m_generation;

};


template <typename E>
class Entity::Handle : public Entity::Reference {
public:
    inline explicit Handle(const Meta::Entity::Controller *c = nullptr);

    inline E *operator->();
    inline const E *operator->() const;

    inline E &operator*();
    inline const E &operator*() const;

    inline E *getEntity();
    inline const E *getEntity() const;

private:
    E *m_entity;

};


template <typename ...C>
class Entity::With : public Entity {
public:
    template <typename ...Args>
    inline explicit With(Args&& ...args);

    template <typename T>
    inline T &get();

private:
    using ComponentPack = typename CDepResolver<PPack<>, PPack<>, PPack<C...>>::Result;

    TypeSet<typename PackOf<ComponentPack>::Pointers> m_components;

    template <typename ...Types>
    inline void findComponents(PPack<Types...>, const Meta::Component::SafePtr *components);

    template <typename ...Args, typename ...Default>
    inline void initialize(PPack<Default...>, Args&& ...args);
};


#endif // SECS_ENTITY_HPP
