#ifndef SECS_ENTITY_ID_HPP
#define SECS_ENTITY_ID_HPP

#include <limits>
#include "engine.hpp"

class EntityID {
public:
    EntityID();

    static const EntityID INVALID;

private:
    template <int INSTANCE_ID>
    friend class EngineInstance<INSTANCE_ID>::EntityManager;

    EntityID(unsigned int id, unsigned int version);

    unsigned int m_id;
    unsigned int m_version;

};

EntityID::EntityID() :
    m_id(EntityID::INVALID.m_version),
    m_version(EntityID::INVALID.m_version) {
}

EntityID::EntityID(unsigned int id, unsigned int version) :
    m_id(id),
    m_version(version) {
}

const EntityID EntityID::INVALID = EntityID(
        std::numeric_limits<unsigned int>::max(),
        std::numeric_limits<unsigned int>::max()
);

#endif // SECS_ENTITY_ID_HPP
