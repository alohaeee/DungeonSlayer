#ifndef ECS_FWD_HPP
#define ECS_FWD_HPP

#include "config.h"
#include "type_traits.hpp"

namespace ssecs::ecs
{

/*! @class basic_registry */
template <typename>
class basic_registry;

/*! @class basic_view */
template <typename...>
class basic_view;

/*! @brief Alias declaration for the most common use case. */
SSECS_OPAQUE_TYPE(entity, SSECS_ID_TYPE);

/*! @brief Alias declaration for the most common use case. */
SSECS_OPAQUE_TYPE(component, SSECS_ID_TYPE);

using registry = basic_registry<entity>;

/**
 * @brief Alias declaration for the most common use case.
 * @tparam Types Types of components iterated by the view.
 */
template <typename... Types>
using view = basic_view<entity, Types...>;
} // namespace ssecs::ecs

#endif // ECS_FWD_HPP
