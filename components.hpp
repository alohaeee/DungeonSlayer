#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SDL.h>
#include <string>

#include "ssecs/ssecs.hpp"
using namespace ssecs;

inline ecs::registry registry{};
inline sdl::TextureLoader s{};

struct Position
{
    float x;
    float y;
};
struct Sprite
{
    SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
};

#endif // COMPONENTS_HPP
