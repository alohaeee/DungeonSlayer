#include "playerinputsystem.hpp"

#include "core.hpp"

void PlayerMovement()
{
    auto state = sdl::Events::KeyboardState();
    auto view = registry.view<Player, Velocity, MovementSpeed, AnimationPool, Sprite>();
    auto &&[vel, speed, animation, sprite] = view.get<Velocity, MovementSpeed, AnimationPool, Sprite>(*view.begin());

    constexpr const auto diagonal = 0.7f;

    if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_D])
    {
        animation.current = "run";
        vel.y = speed.speed * diagonal;
        vel.x = speed.speed * diagonal;
        sprite.isFliped = false;
    }
    else if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_A])
    {
        animation.current = "run";
        vel.y = speed.speed * diagonal;
        vel.x = -speed.speed * diagonal;
        sprite.isFliped = true;
    }
    else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_A])
    {
        animation.current = "run";
        vel.y = -speed.speed * diagonal;
        vel.x = -speed.speed * diagonal;
        sprite.isFliped = true;
    }
    else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_D])
    {
        animation.current = "run";
        vel.y = -speed.speed * diagonal;
        vel.x = speed.speed * diagonal;
        sprite.isFliped = false;
    }
    else if (state[SDL_SCANCODE_W])
    {
        animation.current = "run";
        vel.y = speed.speed;
        vel.x = 0;
    }
    else if (state[SDL_SCANCODE_S])
    {
        animation.current = "run";
        vel.y = -speed.speed;
        vel.x = 0;
    }
    else if (state[SDL_SCANCODE_A])
    {
        animation.current = "run";
        sprite.isFliped = true;
        vel.x = -speed.speed;
        vel.y = 0;
    }
    else if (state[SDL_SCANCODE_D])
    {
        animation.current = "run";
        sprite.isFliped = false;
        vel.x = speed.speed;
        vel.y = 0;
    }
    else
    {
        animation.current = "idle";
        vel.y = 0;
        vel.x = 0;
    }
}
