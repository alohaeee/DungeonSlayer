#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>

#define SSECS_ENTITY_ID_8


namespace ssecs
{

namespace ecs
{

#ifdef SSECS_ENTITY_ID_8
using Entity = std::uint8_t;
#elif SSECS_ENTITY_ID_16
using Entity = std::uint16_t;
#elif SSECS_ENTITY_ID_32
using Entity = std::uint32_t;
#endif

}
}

#endif // ENTITY_H
