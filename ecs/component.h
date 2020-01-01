#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstddef>
#include <cstdint>

namespace ssecs
{
namespace ecs
{

#define SSECS_COMPONENT_ID_SIZE_T
#define SSECS_COMPONENT_TYPE_8

#ifdef SSECS_COMPONENT_ID_SIZE_T
using ComponentID = std::size_t;
#elif SSECS_COMPONENT_ID_8
using ComponentID = std::uint8_t;
#elif SSECS_COMPONENT_ID_16
using ComponentID = std::uint16_t;
#elif SSECS_COMPONENT_ID_32
using ComponentID = std::uint32_t;
#endif

#ifdef SSECS_COMPONENT_TYPE_8
using ComponentType = std::uint8_t;
#elif SSECS_COMPONENT_TYPE_16
using ComponentType = std::uint16_t;
#elif SSECS_COMPONENT_TYPE_32
using ComponentType = std::uint32_t;
#endif

}
}

#endif // COMPONENT_H
