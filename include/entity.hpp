#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP


#include "component.hpp"
#include "meta.hpp"
#include "type_set.hpp"
#include "utility.hpp"

template <typename ...Components>
class Entity : public Meta::Entity::Base {
private:
    template <typename ...C>
    struct CDepResolver {
        using Result = PPack<>;
    };

    template <typename ...N, typename ...P, typename Comp, typename ...U>
    struct CDepResolver<PPack<N...>, PPack<P...>, PPack<Comp, U...>> {
    private:
        using CDep = typename Comp::Dependencies;
        using Need = typename PPack<N...>::template AddUnique<CDep, Comp>::Result;
        using Processed = typename PPack<P...>::template AddUnique<Comp>::Result;
        using Unprocessed = typename PPack<U...>::template AddUnique<CDep>::Result::template Subtract<Processed>::Result;
        using Next = typename CDepResolver<Need, Processed, Unprocessed>::Result;
    public:
        using Result = typename Need::template AddUnique<Next>::Result;
    };

public:
    using ComponentTypes = typename CDepResolver<PPack<>, PPack<>, PPack<Components...>>::Result;
    using BaseType = Entity<Components...>;

    template <typename ...Args>
    explicit Entity(Args&& ...args);

    template <typename C>
    C &get();

private:
    TypeSet<typename PackOf<ComponentTypes>::Pointers> m_components;

    template <typename ...Types>
    void findComponents(PPack<Types...>, const Meta::Component::SafePtr *components);

    template <typename ...Args, typename ...Default>
    void initialize(PPack<Default...>, Args&& ...args);
};


#endif // SECS_ENTITY_HPP
