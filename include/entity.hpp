#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP


#include "utility.hpp"
#include "type_set.hpp"

class EntityBase : public Immovable {
public:
    using Destructor = void(*)(EntityBase *);

    inline EntityBase();
    inline ~EntityBase();

    inline bool init(Destructor destructor);
    inline bool destroy();

private:
    bool m_alive;
    unsigned m_generation;
    Destructor m_destructor;

    friend class EntityHandle;
};

class EntityHandle {
public:
    inline explicit EntityHandle(EntityBase *base = nullptr);
    inline bool alive() const;

private:
    unsigned m_generation;
    EntityBase *m_entity;

    template <typename Application>
    friend class EntityManager;

};

template <typename ...Components>
class Entity : public EntityBase {
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

    Entity();

    template <typename T>
    T *get();

private:
    TypeSet<typename PackOf<ComponentTypes>::Pointers> m_components;

};


#endif // SECS_ENTITY_HPP
