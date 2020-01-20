#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL_rect.h>
#include <SDL_render.h>
#include <cstdint>
#include <string>
#include <vector>

#include "tileset.hpp"
#include "vector2d.hpp"

namespace ssecs::component
{

struct Sprite
{
    SDL_Texture *texture = nullptr;
    SDL_Rect rect{};
    Vector2D scale{1, 1};
    std::size_t layer = 0;
    bool isFliped = false;
};

struct Animation
{
    std::vector<SDL_Rect> data;
    float rate;
    std::size_t current;
    float time;
};

struct AnimationPool
{
    std::map<std::string, Animation> data;
    std::string current;
    bool isPlaying = true;
};

} // namespace ssecs::component

#endif // SPRITE_HPP
