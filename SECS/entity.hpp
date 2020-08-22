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

private:

};

#endif // SECS_ENTITY_HPP

#ifndef SECS_ENTITY_INL
#include "impl/entity.inl"
#endif
