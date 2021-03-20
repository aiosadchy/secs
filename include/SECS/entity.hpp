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


namespace secs {

class Entity {
private:
    static const Entity &get_null() noexcept;

public:
    using ID = std::conditional_t<
        (SECS_ENTITY_INDEX_BITS + SECS_ENTITY_VERSION_BITS > 32),
        std::uint64_t,
        std::uint32_t
    >;

    inline Entity() noexcept;
    inline ID get_id() const noexcept;
    inline bool operator==(Entity another) const noexcept;
    inline bool operator!=(Entity another) const noexcept;

    inline static const Entity& null = get_null();

private:
    friend class EntityPool;

    static constexpr ID INDEX_BITS     = SECS_ENTITY_INDEX_BITS;
    static constexpr ID VERSION_BITS   = SECS_ENTITY_VERSION_BITS;
    static constexpr ID INDEX_OFFSET   = VERSION_BITS;
    static constexpr ID VERSION_OFFSET = 0;
    static constexpr ID INDEX_MASK     = (ID(1) << INDEX_BITS  ) - ID(1);
    static constexpr ID VERSION_MASK   = (ID(1) << VERSION_BITS) - ID(1);
    static constexpr ID USED_BITS_MASK = (  INDEX_MASK << INDEX_OFFSET  )
                                       | (VERSION_MASK << VERSION_OFFSET);

    inline Entity(Index index, Index version) noexcept;
    inline Index get_index() const noexcept;
    inline Index get_version() const noexcept;

    ID m_data;

};

} // namespace secs

#endif // SECS_ENTITY_HPP
