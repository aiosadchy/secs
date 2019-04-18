#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP


#include "meta.hpp"
#include "type_set.hpp"
#include "utility.hpp"

class Entity : public Immovable {
public:
    using TypeID = Meta::Entity::TypeID;

    using Reference = Meta::Entity::Reference;

    template <typename E>
    using Handle = Meta::Entity::Handle<E>;

    template <typename ...Components>
    class With;

protected:
    Entity() = default;
    ~Entity() = default;

    const Meta::Entity::Controller *getController() const;

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

    template <typename ...>
    class ComponentSet;

    template <typename ...C>
    class ComponentSet<PPack<C...>> : public TypeSet<Meta::Component::Controller<C> * ...> {
        template <typename T>
        inline Meta::Component::Controller<T> *get();
    };

};


template <typename ...Components>
class Entity::With : public Entity {
public:
    template <typename ...Args>
    inline explicit With(Args&& ...args);

    inline ~With();

    template <typename T>
    inline T &get();

private:
    using ComponentTypes = typename CDepResolver<PPack<>, PPack<>, PPack<Components...>>::Result;

    ComponentSet<ComponentTypes> m_components;

    template <typename ...C>
    inline void findComponents(PPack<C...>, const Meta::Component::SafePtr *components);

    template <typename ...Args, typename ...Default>
    inline void initialize(const Reference &entity, PPack<Default...>, Args&& ...args);

};


#endif // SECS_ENTITY_HPP
