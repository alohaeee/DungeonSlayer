#ifndef COLLIDERS_HPP
#define COLLIDERS_HPP

#include "../../entt/entt.hpp"
#include <SDL_rect.h>
#include <map>
#include <string>

namespace ssecs::component
{
struct RectCollider
{
    SDL_FRect rect;
    inline static bool hasDebugDraw = true;

    // entt::sigh<void(RectCollider, RectCollider)> signal;
};
struct TileGridCollider
{
};
struct Name
{
    std::string name;
};
} // namespace ssecs::component

#endif // COLLIDERS_HPP
