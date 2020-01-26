#include "playerinputsystem.hpp"

#include "../core.hpp"
#include "collisionsystem.hpp"
#include "spritesystem.hpp"

void PlayerMovement(const float dt)
{
    auto state = sdl::Events::KeyboardState();
    auto view = registry.view<Player, Velocity, MovementSpeed, AnimationPool, Sprite, Dash, Active>();
    if (view.begin() != view.end())
    {
        auto &&[vel, speed, animation, sprite, dash] =
            view.get<Velocity, MovementSpeed, AnimationPool, Sprite, Dash>(*view.begin());

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
        if (state[SDL_SCANCODE_RCTRL] && dash.canDashing)
        {
            dash.canDashing = false;
            dash.dt = 0;
            auto label_view = registry.view<Label, entt::tag<"dash"_hs>>();
            auto &label = label_view.get<Label>(*label_view.begin());
            label.AssignTexture(textureCache.resource("dash_no"));
        }
        if (!dash.canDashing)
        {
            dash.dt += dt;
            if (dash.dt < dash.duration)
            {
                vel.x *= dash.power;
                vel.y *= dash.power;
            }
            if (dash.dt >= dash.cd)
            {
                dash.canDashing = true;
                auto label_view = registry.view<Label, entt::tag<"dash"_hs>>();
                auto &label = label_view.get<Label>(*label_view.begin());
                label.AssignTexture(textureCache.resource("dash_ok"));
            }
        }
    }
}

void PlayerAttack(const float dt)
{
    auto state = sdl::Events::KeyboardState();
    auto view = registry.view<Player, Hierarchy, Position, Sprite, Velocity, RectCollider, Active>();
    if (view.begin() != view.end())
    {

        auto &&[parent, pos, sprite, vel, player, collider] =
            view.get<Hierarchy, Position, Sprite, Velocity, Player, RectCollider>(*view.begin());

        auto &attack = parent.child;

        auto &&[attack_pos, attack_animation, attack_sprite, attack_rect] =
            registry.get<Position, AnimationPool, Sprite, RectCollider>(attack);
        if (state[SDL_SCANCODE_SPACE] && !player.isAttacking)
        {
            player.isAttacking = true;
            player.isHit = true;
            player.dt = 0;

            attack_animation.isPlaying = true;
            attack_animation.data.at(attack_animation.current).current = 0;
            attack_animation.data.at(attack_animation.current).time = 0;
            attack_sprite.isHidden = false;
            attack_sprite.isFliped = sprite.isFliped;
            player.attack_direction.Set(vel.x, vel.y);
        }
        if (player.isAttacking)
        {
            player.dt += dt;
            if (attack_animation.data.at(attack_animation.current).current ==
                attack_animation.data.at(attack_animation.current).data.size())
            {
                attack_sprite.isHidden = true;
                attack_animation.isPlaying = false;
                player.isHit = false;
            }

            if (player.attack_direction.x() < 0)
            {
                attack_pos.position.Set(pos.position.x() - attack_rect.rect.w, pos.position.y());
            }
            else if (player.attack_direction.x() > 0)
            {
                attack_pos.position.Set(pos.position.x() + collider.rect.w, pos.position.y());
            }
            else if (player.attack_direction.y() == 0)
            {
                if (attack_sprite.isFliped)
                {
                    attack_pos.position.Set(pos.position.x() - attack_rect.rect.w, pos.position.y());
                }
                else
                {
                    attack_pos.position.Set(pos.position.x() + collider.rect.w, pos.position.y());
                }
            }
            else
            {
                attack_pos.position = pos.position;
            }

            if (player.attack_direction.y() < 0)
            {
                attack_pos.position.Set(attack_pos.position.x(), pos.position.y() - attack_rect.rect.h);
            }
            else if (player.attack_direction.y() > 0)
            {
                attack_pos.position.Set(attack_pos.position.x(), pos.position.y() + collider.rect.h);
            }

            if (player.dt > player.attack_rate)
            {
                player.isAttacking = false;
            }
        }
    }
}

void ActiveGame()
{
    auto enemyView = registry.view<Enemy, Hierarchy, Health>();
    for (auto &entt : enemyView)
    {
        registry.assign<Active>(entt);
        auto &hierarchy = enemyView.get<Hierarchy>(entt);
        if (hierarchy.child != entt::null)
        {
            registry.assign<Active>(hierarchy.child);
        }
    }
    auto playerView = registry.view<Player, Hierarchy, Position, Health>();
    auto &&[player, hierarchy, health, pos] = registry.get<Player, Hierarchy, Health, Position>(*playerView.begin());
    registry.assign<Active>(*playerView.begin());
    if (hierarchy.child != entt::null)
    {
        registry.assign<Active>(hierarchy.child);
    }
    health.health = 3;
    player.score = 0;
    pos.position = {550, 746};
    SetPlayerScore(0);
    SetPlayerHealth(3);

    auto score = registry.view<entt::tag<"score"_hs>>();
    registry.assign<Active>(*score.begin());
    auto hp = registry.view<entt::tag<"hp"_hs>>();
    registry.assign<Active>(*hp.begin());
    auto dash = registry.view<entt::tag<"dash"_hs>>();
    registry.assign<Active>(*dash.begin());

    auto table_view = registry.view<entt::tag<"table"_hs>>();
    for (auto &entt : table_view)
    {
        registry.remove<Active>(entt);
    }
}
void CloseGame()
{
    auto enemyView = registry.view<Enemy, Hierarchy, Health>();
    for (auto &entt : enemyView)
    {
        registry.remove<Active>(entt);
        auto &hierarchy = enemyView.get<Hierarchy>(entt);
        if (hierarchy.child != entt::null)
        {
            registry.remove<Active>(hierarchy.child);
        }
        auto &health = enemyView.get<Health>(entt);
        health.health = 0;
    }
    auto playerView = registry.view<Player, Hierarchy>();
    auto &&[player, hierarchy] = registry.get<Player, Hierarchy>(*playerView.begin());
    registry.remove<Active>(*playerView.begin());
    if (hierarchy.child != entt::null)
    {
        registry.remove<Active>(hierarchy.child);
    }

    auto score = registry.view<entt::tag<"score"_hs>>();
    registry.remove<Active>(*score.begin());
    auto hp = registry.view<entt::tag<"hp"_hs>>();
    registry.remove<Active>(*hp.begin());
    auto dash = registry.view<entt::tag<"dash"_hs>>();
    registry.remove<Active>(*dash.begin());

    for (auto &row : scoreTable.table)
    {
        if (row.second < player.score)
        {
            row.first = player.name;
            row.second = player.score;
            break;
        }
    }
    auto table_view = registry.view<entt::tag<"table"_hs>>();
    for (auto &entt : table_view)
    {

        registry.assign<Active>(entt);
    }
    ResetTable();
    isMenu = true;
}

void OpenGame()
{
    if (isMenu)
    {
        auto state = sdl::Events::KeyboardState();
        auto event = sdl::Events::Event();
        auto view = registry.view<Player>();
        auto &player = view.get<Player>(*view.begin());
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
        {
            if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z)
            {
                if (player.name.size() < 3)
                {
                    player.name += event.key.keysym.sym;

                    ResetName();
                }
            }
            if (event.key.keysym.sym == SDLK_BACKSPACE)
            {
                if (player.name.size() > 0)
                {
                    player.name.pop_back();
                    ResetName();
                }
            }
        }
        if (state[SDL_SCANCODE_RETURN])
        {
            isMenu = false;
            ActiveGame();
        }
    }
}
