#ifndef HIERACHY_HPP
#define HIERACHY_HPP

#include "../../entt/entity/entity.hpp"
#include "../../entt/fwd.hpp"

namespace ssecs::component
{
struct Hierarchy
{
    entt::entity child{entt::null};
    entt::entity parent{entt::null};
};

struct Parent
{
    entt::entity parent;
};
} // namespace ssecs::component

#endif // HIERACHY_HPP
