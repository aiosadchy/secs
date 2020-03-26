#ifndef SECS_ENGINE_INL
#define SECS_ENGINE_INL

#include "SECS/engine.hpp"

template <typename InstanceID>
EngineInstance<InstanceID> EngineInstance<InstanceID>::instance = EngineInstance();

#endif // SECS_ENGINE_INL
