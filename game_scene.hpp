#ifndef GAME_PROCESS_HPP
#define GAME_PROCESS_HPP

#include "core.hpp"
#include "systems/systems.hpp"

#include <filesystem>
#include <iostream>

#include "xml/pugixml.hpp"

constexpr const auto tileid = "tileset";
constexpr const auto spriteid = "spritesheet";
class GameScene : public sdl::BasicScene
{
public:
    GameScene()
    {
        textureCache.load(tileid, sdl::TextureLoader::Sprite("tilemap.png"));
        textureCache.load(spriteid, sdl::TextureLoader::Sprite("spritesheet.png"));

        spriteSheet.Load("sprite.tsx", textureCache.resource(spriteid));
        tileset.Load("tile.tsx", textureCache.resource(tileid));
        CameraCreate();
        GridCreate();
        SpriteCreate();
    }

    void FixedUpdate() override
    {
        CollisionDetection();
        CameraFollow();
    }
    void Update([[maybe_unused]] const float dt) override
    {
        AnimationUpdate(dt);
        MovementUpdate(dt);
    }
    void InputUpdate() override
    {
        CameraUpdateDebug();
        PlayerMovement();
    }
    void Render() override
    {
        GridRender();
        SpriteRender();
        PositionDebug();
    }

private:
};

#endif // GAME_PROCESS_HPP
