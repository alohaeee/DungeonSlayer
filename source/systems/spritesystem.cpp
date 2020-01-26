#include "spritesystem.hpp"

#include "../core.hpp"

void SpriteRender()
{
    auto view = registry.view<Sprite, Position, Active>();
    auto cameraView = registry.view<Camera>();
    auto camera = cameraView.get(*cameraView.begin());

    for (const auto &entity : view)
    {
        Sprite sprite = view.get<Sprite>(entity);
        if (!sprite.isHidden)
        {
            Position position = view.get<Position>(entity);

            SDL_FRect rect{position.position.x(), position.position.y(), sprite.rect.w * sprite.scale.x(),
                           sprite.rect.h * sprite.scale.y()};

            if (camera.Contains(rect))
            {
                SDL_Rect dst_rect = camera.FromWorldToScreenRect(rect);
                SDL_SetTextureAlphaMod(sprite.texture, sprite.color.a);

                sdl::Graphics::RenderToLayer(sprite.layer, sprite.texture, &sprite.rect, &dst_rect,
                                             (sprite.isFliped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));

                SDL_SetTextureAlphaMod(sprite.texture, SDL_ALPHA_OPAQUE);
            }
        }
    };
}

void PlayerCreate()
{
    auto id = registry.create();
    auto &sprite = registry.assign<Sprite>(id);
    auto &pos = registry.assign<Position>(id);
    registry.assign<Velocity>(id);
    auto &animation = registry.assign<AnimationPool>(id);
    registry.assign<Player>(id);
    registry.assign<Health>(id, 3);
    registry.assign<Active>(id);
    auto &speed = registry.assign<MovementSpeed>(id);
    auto &rect = registry.assign<RectCollider>(id);

    registry.assign<NullVelocityCollision>(id);
    registry.assign<CollisionLayer>(id, LayersID::PLAYER);
    registry.assign<ParticleData>(id);
    registry.assign<Dash>(id);

    auto idle = spriteSheet.GetTypeFamily("knight_idle");
    auto run = spriteSheet.GetTypeFamily("knight_run");

    animation.data.emplace("idle", Animation{idle, 0.07f, 0, 0});
    animation.data.emplace("run", Animation{run, 0.07f, 0, 0});
    animation.current = "idle";
    animation.isPlaying = true;

    pos.position = {550, 746};

    sprite.texture = textureCache.resource("spritesheet");

    sprite.scale = {2, 2};
    sprite.rect = spriteSheet[spriteSheet("knight_idle").first->second];
    sprite.layer = 2;
    sprite.isFliped = false;

    rect.rect.w = sprite.rect.w * sprite.scale.x();
    rect.rect.h = sprite.rect.h * sprite.scale.y();

    speed.speed = 400.f;

    auto attack = registry.create();
    [[maybe_unused]] auto &attack_pos = registry.assign<Position>(attack);
    auto &attack_rect = registry.assign<RectCollider>(attack);
    auto &attack_animation = registry.assign<AnimationPool>(attack);
    registry.assign<Active>(attack);

    registry.assign<CollisionLayer>(attack, LayersID::PLAYER);

    auto &attack_hierarchy = registry.assign<Hierarchy>(attack);
    attack_hierarchy.parent = id;
    auto &parent = registry.assign<Hierarchy>(id);
    parent.child = attack;

    auto &attack_sprite = registry.assign<Sprite>(attack);
    attack_sprite.texture = textureCache.resource("spritesheet");

    attack_sprite.scale = {2, 2};
    attack_sprite.rect = spriteSheet[spriteSheet("attack").first->second];
    attack_sprite.layer = 2;
    attack_sprite.isFliped = false;
    attack_sprite.isHidden = true;

    attack_rect.rect.w = attack_sprite.scale.x() * attack_sprite.rect.w;
    attack_rect.rect.h = attack_sprite.scale.y() * attack_sprite.rect.h;

    auto animation_type = spriteSheet.GetTypeFamily("attack");

    attack_animation.data.emplace("attack", Animation{animation_type, 0.05f, 0, 0});

    attack_animation.current = "attack";
    attack_animation.isPlaying = false;
}

void AnimationUpdate(const float dt)
{
    auto view = registry.view<Sprite, Position, AnimationPool, Active>();
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
            animation.current++;
        }
    }
}
void ParticleUpdate(const float dt)
{
    auto view = registry.view<ParticleData, Sprite, Active>();

    view.each([dt](auto &particle, auto &sprite, const auto &) {
        if (particle.isPlaying)
        {
            particle.dt += dt;
            if (particle.dt > particle.rate)
            {
                particle.isPlaying = false;
                particle.dt = 0;
                sprite.color.a = SDL_ALPHA_OPAQUE;
            }
            else
            {
                if (sprite.color.a >= SDL_ALPHA_OPAQUE && !particle.isTranspereting)
                {
                    particle.isTranspereting = !particle.isTranspereting;
                }
                else if (sprite.color.a <= 20 && particle.isTranspereting)
                {
                    particle.isTranspereting = !particle.isTranspereting;
                }
                sprite.color.a += (particle.isTranspereting ? -particle.step : particle.step);
                sprite.color.r = 255;
            }
        }
    });
}

void LabelsRender()
{
    auto view = registry.view<Label, Active>();
    auto cameraView = registry.view<Camera>();
    auto camera = cameraView.get(*cameraView.begin());

    for (auto &entt : view)
    {
        auto &label = view.get<Label>(entt);
        if (camera.Contains(label.dst_rect))
        {
            SDL_SetTextureAlphaMod(label.texture, 255);
            sdl::Graphics::RenderToLayer(6, label.texture, &label.src_rect, &label.dst_rect);
        }
    };
}

void CreateLabels()
{
    {
        auto id = registry.create();
        registry.assign<entt::tag<"dash"_hs>>(id);
        auto &label = registry.assign<Label>(id);
        registry.assign<Active>(id);

        textureCache.load("dash_ok",
                          sdl::ResourceLoader::Text(fontCache.resource("font23"), "D", {255, 255, 255, 255}));
        textureCache.load("dash_no",
                          sdl::ResourceLoader::Text(fontCache.resource("font23"), "X", {255, 255, 255, 255}));
        label.texture = textureCache.resource("dash_ok");
        label.dst_rect.x = 580;
        label.dst_rect.y = 580;

        label.AssignTexture(textureCache.resource("dash_ok"));
    }
    {
        auto id = registry.create();
        registry.assign<entt::tag<"hp"_hs>>(id);
        auto &label = registry.assign<Label>(id);
        registry.assign<Active>(id);

        textureCache.load("hp_3",
                          sdl::ResourceLoader::Text(fontCache.resource("font35"), "HP 3", {255, 255, 255, 255}));
        textureCache.load("hp_2",
                          sdl::ResourceLoader::Text(fontCache.resource("font35"), "HP 2", {255, 255, 255, 255}));
        textureCache.load("hp_1",
                          sdl::ResourceLoader::Text(fontCache.resource("font35"), "HP 1", {255, 255, 255, 255}));
        label.texture = textureCache.resource("hp_3");
        label.dst_rect.x = 540;
        label.dst_rect.y = 0;

        label.AssignTexture(textureCache.resource("hp_3"));
    }
    {
        auto id = registry.create();
        registry.assign<entt::tag<"score"_hs>>(id);
        auto &label = registry.assign<Label>(id);
        registry.assign<Active>(id);

        for (int i = 0; i < 100; i++)
        {
            textureCache.load(std::to_string(i), sdl::ResourceLoader::Text(fontCache.resource("font35"),
                                                                           std::to_string(i), {255, 255, 255, 255}));
        }

        label.texture = textureCache.resource("0");
        label.dst_rect.x = 0;
        label.dst_rect.y = 0;

        label.AssignTexture(textureCache.resource("0"));
    }
    for (int i = 0; i < ScoreTable::TABLE_SIZE; i++)
    {
        auto id = registry.create();
        registry.assign<entt::tag<"table"_hs>>(id);
        auto &label = registry.assign<Label>(id);

        label.dst_rect.x = 100;
        label.dst_rect.y = 100 * (i + 1);
    }
    {
        auto id = registry.create();
        registry.assign<entt::tag<"name"_hs>>(id);
        auto &label = registry.assign<Label>(id);
        registry.assign<Active>(id);

        auto view = registry.view<Player>();
        auto &player = view.get<Player>(*view.begin());
        textureCache.load("name",
                          sdl::ResourceLoader::Text(fontCache.resource("font23"), player.name, {255, 255, 255, 255}));

        label.AssignTexture(textureCache.resource("name"));
        label.dst_rect.x = 0;
        label.dst_rect.y = 580;
    }
}

void ResetTable()
{
    auto view = registry.view<entt::tag<"table"_hs>, Label>();
    int i = ScoreTable::TABLE_SIZE - 1;
    for (auto &entt : view)
    {
        auto &label = view.get<Label>(entt);
        std::string text;
        if (scoreTable.table[i].second > 0)
        {
            text = scoreTable.table[i].first + "   " + std::to_string(scoreTable.table[i].second);
        }
        else
        {
            text = " ";
        }
        textureCache.load("table" + std::to_string(i),
                          sdl::ResourceLoader::Text(fontCache.resource("font35"), text, {255, 255, 255, 255}));
        label.AssignTexture(textureCache.resource("table" + std::to_string(i)));
        i--;
    }
}

void ResetName()
{
    auto view = registry.view<entt::tag<"name"_hs>, Label>();
    auto &label = view.get<Label>(*view.begin());

    auto view_player = registry.view<Player>();
    for (auto &entt : view_player)
    {
        auto &player = view_player.get<Player>(entt);
        textureCache.load("name",
                          sdl::ResourceLoader::Text(fontCache.resource("font23"),
                                                    (player.name.empty() ? " " : player.name), {255, 255, 255, 255}));

        label.AssignTexture(textureCache.resource("name"));
    }
}
