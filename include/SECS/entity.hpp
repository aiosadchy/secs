#ifndef SECS_ENTITY_HPP
#define SECS_ENTITY_HPP

#include <cstdint>
#include <type_traits>

#include "SECS/common.hpp"

#ifndef SECS_ENTITY_INDEX_BITS
    #define SECS_ENTITY_INDEX_BITS 26
#endif

#ifndef SECS_ENTITY_VERSION_BITS
    #define SECS_ENTITY_VERSION_BITS 6
#endif


class Entity {
public:
    class ID {
    public:
        inline ID() noexcept;
        inline ID(Index index, Index version) noexcept;

        inline Index get_index() const noexcept;
        inline Index get_version() const noexcept;

        inline bool operator==(const ID &another) const noexcept;
        inline bool operator!=(const ID &another) const noexcept;

    private:
        using UnderlyingType = std::conditional_t<
            (SECS_ENTITY_INDEX_BITS + SECS_ENTITY_VERSION_BITS > 32),
            std::uint64_t,
            std::uint32_t
        >;

        static constexpr UnderlyingType INDEX_BITS   = SECS_ENTITY_INDEX_BITS;
        static constexpr UnderlyingType VERSION_BITS = SECS_ENTITY_VERSION_BITS;
        static constexpr UnderlyingType INDEX_OFFSET   = VERSION_BITS;
        static constexpr UnderlyingType VERSION_OFFSET = 0;
        static constexpr UnderlyingType INDEX_MASK   = (1u << INDEX_BITS  ) - 1u;
        static constexpr UnderlyingType VERSION_MASK = (1u << VERSION_BITS) - 1u;

        UnderlyingType m_data;

    };

    inline static const ID null = ID();

};

#endif // SECS_ENTITY_HPP
