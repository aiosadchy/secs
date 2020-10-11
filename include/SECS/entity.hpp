#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP

#include <limits>
#include "SECS/utility/types.hpp"

class Entity {
public:
    class ID {
    public:
        inline ID();
        inline ID(Index index, Index version);

        inline Index index() const;
        inline Index version() const;

    private:
        Index m_index;
        Index m_version;

    };

    class Reference {
    public:
        template <typename Component, typename ...Args>
        Component &assign(Args&& ...args);

        template <typename ...Component>
        [[nodiscard]] bool has() const;

        template <typename ...Component>
        [[nodiscard]] decltype(auto) get();

        template <typename ...Component>
        [[nodiscard]] decltype(auto) get() const;

        template <typename ...Component>
        [[nodiscard]] decltype(auto) find();

        template <typename ...Component>
        [[nodiscard]] decltype(auto) find() const;

        template <typename ...Component>
        void remove();

    private:
        friend class Engine;

        ID m_id;

    };

private:

};

#endif // SECS_ENTITY_HPP

#include "impl/entity.inl"
