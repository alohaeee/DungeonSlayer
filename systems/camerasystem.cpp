#include "camerasystem.hpp"

#include "core.hpp"

void CameraCreate()
{
    auto id = registry.create();
    auto &camera = registry.assign<Camera>(id);
    camera.position = {500, 500};
    camera.viewRadius = {500, 500};
    camera.UpdateWindowSize(sdl::Graphics::Window());
}

void CameraUpdateDebug()
{
    if (!CameraData::isFollowing)
    {
        auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());
        auto event = sdl::Events::Event();
        if (event.key.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                activeCamera.position.Set(activeCamera.position.x(),
                                          activeCamera.position.y() + activeCamera.viewRadius.y() / 10);
                break;
            case SDLK_DOWN:
                activeCamera.position.Set(activeCamera.position.x(),
                                          activeCamera.position.y() - activeCamera.viewRadius.y() / 10);
                break;
            case SDLK_LEFT:
                activeCamera.position.Set(activeCamera.position.x() - activeCamera.viewRadius.x() / 10,
                                          activeCamera.position.y());
                break;
            case SDLK_RIGHT:
                activeCamera.position.Set(activeCamera.position.x() + activeCamera.viewRadius.x() / 10,
                                          activeCamera.position.y());
                break;
            }
        }
    }
}

void CameraFollow()
{
    if (CameraData::isFollowing)
    {
        auto playerView = registry.view<Player, Position, Sprite>();
        auto &&[position, sprite] = playerView.get<Position, Sprite>(*playerView.begin());
        auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());

        activeCamera.position.Set(position.position.x() + sprite.rect.w * sprite.scale.x() / 2,
                                  position.position.y() + sprite.rect.h * sprite.scale.y() / 2);
    }
}
