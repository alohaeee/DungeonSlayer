#include "spritesystem.hpp"

#include "core.hpp"

void SpriteRender()
{
    auto view = registry.view<Sprite, Position>();
    auto cameraView = registry.view<Camera>();
    auto camera = cameraView.get(*cameraView.begin());

    for (const auto &entity : view)
    {
        Sprite sprite = view.get<Sprite>(entity);
        Position position = view.get<Position>(entity);

        SDL_FRect rect{position.position.x(), position.position.y(), sprite.rect.w * sprite.scale.x(),
                       sprite.rect.h * sprite.scale.y()};

        if (camera.Contains(rect))
        {
            SDL_Rect dst_rect = camera.FromWorldToScreenRect(rect);

            sdl::Graphics::RenderToLayer(sprite.layer, sprite.texture, &sprite.rect, &dst_rect,
                                         (sprite.isFliped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
        }
    };
}

void SpriteCreate()
{
    auto id = registry.create();
    auto &sprite = registry.assign<Sprite>(id);
    auto &pos = registry.assign<Position>(id);
    registry.assign<Velocity>(id);
    auto &animation = registry.assign<AnimationPool>(id);
    registry.assign<Player>(id);
    auto &speed = registry.assign<MovementSpeed>(id);
    auto &rect = registry.assign<RectCollider>(id);

    auto idle = spriteSheet.GetTypeFamily("knight_idle");
    auto run = spriteSheet.GetTypeFamily("knight_run");

    animation.data.emplace("idle", Animation{idle, 0.15f, 0, 0});
    animation.data.emplace("run", Animation{run, 0.15f, 0, 0});
    animation.current = "idle";
    animation.isPlaying = true;

    pos.position = {300, 300};

    sprite.texture = textureCache.resource("spritesheet");

    sprite.scale = {2, 2};
    sprite.rect = spriteSheet[spriteSheet("knight_idle").first->second];
    sprite.layer = 2;
    sprite.isFliped = false;

    rect.rect.w = sprite.rect.w * sprite.scale.x();
    rect.rect.h = sprite.rect.h * sprite.scale.y();

    speed.speed = 180.f;
}

void AnimationUpdate(const float dt)
{
    auto view = registry.view<Sprite, Position, AnimationPool>();
    auto cameraView = registry.view<Camera>();
    auto camera = cameraView.get(*cameraView.begin());

    for (auto &entity : view)
    {

        auto &animationPool = view.get<AnimationPool>(entity);
        auto &animation = animationPool.data.at(animationPool.current);
        auto &sprite = view.get<Sprite>(entity);
        if (animation.current >= animation.data.size())
        {
            animation.current = 0;
        }
        animation.time += dt;
        if (animation.time > animation.rate)
        {
            sprite.rect = animation.data[animation.current];
            animation.time = 0;
        }
        animation.current++;
    }
}
