#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP


#include "utility.hpp"
#include "type_set.hpp"

template <typename ...Components>
class Entity : public Immovable {
public:
    template <typename T>
    T *get();

protected:
    Entity();

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

    template <typename ...>
    struct ComponentSet;

    template <typename ...C>
    struct ComponentSet<PPack<C...>> {
        using Type = TypeSet<C*...>;
    };

    using ComponentPack = typename CDepResolver<PPack<>, PPack<>, PPack<Components...>>::Result;
    using ComponentHolder = typename ComponentSet<ComponentPack>::Type;

    ComponentHolder m_components;

};


#endif // SECS_ENTITY_HPP
