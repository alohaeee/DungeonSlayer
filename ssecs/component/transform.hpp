#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "vector2d.hpp"
namespace ssecs::component
{
struct Position
{
    Vector2D position;
    inline static bool hasDebugDraw = true;
};
struct Velocity
{
    float x;
    float y;
};
struct MovementSpeed
{
    float speed;
};

struct Player
{
};

class Transform
{
public:
private:
    Vector2D position;
    Vector2D scale;
    // Vector2D rotation;
};
} // namespace ssecs::component

#endif // TRANSFORM_HPP
