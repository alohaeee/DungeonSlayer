#ifndef ECS_UTILITY_HPP
#define ECS_UTILITY_HPP

#include "type_traits.hpp"

namespace ssecs::ecs
{

/**
 * @brief Alias for exclusion lists.
 * @tparam Type List of types.
 */
template <typename... Type>
struct exclude_t : type_list<Type...>
{
};

/**
 * @brief Variable template for exclusion lists.
 * @tparam Type List of types.
 */
template <typename... Type>
[[maybe_unused]] constexpr exclude_t<Type...> exclude{};
} // namespace ssecs::ecs

#endif // ECS_UTILITY_HPP
