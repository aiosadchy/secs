#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP


template <typename... Components>
class Entity {
protected:
    Entity();

private:
    template <typename C>
    static constexpr bool checkRequirements();

    template <typename First, typename Second, typename... Rest>
    static constexpr bool checkRequirements();

};


#endif // SECS_ENTITY_HPP
