#ifndef AI_HPP
#define AI_HPP

#include "vector2d.hpp"
#include <array>
namespace ssecs::component
{

struct Pathfinding
{
    inline static constexpr const auto DIAGONAL_COST = 14;
    inline static constexpr const auto SIDE_COST = 10;
};

struct Enemy
{
    inline static int currentSpawn = 0;
    inline static constexpr const auto MAX_SPAWNS = 8;
    inline static const Vector2D spawns[MAX_SPAWNS]{
        {250, 250},
        {500, 500},
        {100, 400},
        {974.667847, 716.000488},
        {600, 140},
        {1100.667603, 271.333405},
        {1135.333862, 786.667297},
        {1135.333862, 200},
    };
    bool isCharched = false;
    float time = 0.5;
    float dt = 0;
};
struct Health
{
    int health = 2;
};
struct Active
{
};

struct View
{
    float multiplier;
};
} // namespace ssecs::component
#endif // AI_HPP
