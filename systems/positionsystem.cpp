#include "positionsystem.hpp"

#include "core.hpp"

void MovementUpdate(float dt)
{
    auto view = registry.view<Position, Velocity>();

    for (auto &entity : view)
    {
        auto &&[pos, vel] = view.get<Position, Velocity>(entity);
        pos.position.Set(pos.position.x() + vel.x * dt, pos.position.y() + vel.y * dt);
    }
}
