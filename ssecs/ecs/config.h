#ifndef ECS_CONFIG_H
#define ECS_CONFIG_H

#ifndef SSECS_PAGE_SIZE
// Define page size for pooling component.
#define SSECS_PAGE_SIZE 32768
#endif // SSECS_PAGE_SIZE

#ifndef SSECS_ID_TYPE
#include <cstdint>
#define SSECS_ID_TYPE std::uint16_t
#endif // SSECS_ID_TYPE

#ifndef SSECS_DISABLE_ASSERT
#include <cassert>
#define SSECS_ASSERT(condition) assert(condition)
#else // SSES_DISABLE_ASSERT
#define SSECS_ASSERT(...) ((void)0)
#endif // SSECS_DISABLE_ASSERT

#endif // ECS_CONFIG_H
