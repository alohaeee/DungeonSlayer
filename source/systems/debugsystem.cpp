#include "debugsystem.hpp"

#include "../core.hpp"

void PositionDebug()
{
    if (Position::hasDebugDraw)
    {
        auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());
        auto view = registry.view<Position, Active>();

        view.each([activeCamera](const auto &pos, const auto &) {
            sdl::Graphics::SetDrawColor(255, 0, 0, SDL_ALPHA_OPAQUE);

            auto position = activeCamera.FromWorldToScreenView(pos.position);
            sdl::Graphics::DrawLineToLayer(5, position.x(), position.y(), position.x() + 20, position.y());

            sdl::Graphics::SetDrawColor(0, 255, 0, SDL_ALPHA_OPAQUE);
            sdl::Graphics::DrawLineToLayer(5, position.x(), position.y(), position.x(), position.y() - 20);
            sdl::Graphics::ResetDrawColor();
        });
    }
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
            case SDLK_PAGEUP:
                activeCamera.viewRadius.Set(activeCamera.viewRadius.x() + activeCamera.viewRadius.x() / 10,
                                            activeCamera.viewRadius.y() + activeCamera.viewRadius.y() / 10);
                break;
            case SDLK_PAGEDOWN:
                activeCamera.viewRadius.Set(activeCamera.viewRadius.x() - activeCamera.viewRadius.x() / 10,
                                            activeCamera.viewRadius.y() - activeCamera.viewRadius.y() / 10);
                break;
            }
        }
    }
}

void RectDebug()
{
    if (RectCollider::hasDebugDraw)
    {
        auto cameraView = registry.view<Camera>();
        auto &activeCamera = cameraView.get(*cameraView.begin());
        auto view = registry.view<Position, RectCollider, Active>();

        view.each([activeCamera](const auto &pos, const auto &rect, const auto &act) {
            sdl::Graphics::SetDrawColor(255, 0, 0, SDL_ALPHA_OPAQUE);

            SDL_FRect frect = {pos.position.x() + rect.rect.x, pos.position.y() + rect.rect.y, rect.rect.w,
                               rect.rect.h};
            auto position = activeCamera.FromWorldToScreenRect(frect);
            sdl::Graphics::SetDrawColor(255, 255, 0, 120);
            sdl::Graphics::DrawFillRectToLayer(5, &position);
            sdl::Graphics::SetDrawColor(0, 255, 255, 255);
            sdl::Graphics::DrawRectToLayer(5, &position);
            sdl::Graphics::ResetDrawColor();
        });
    }
}

void DebugMode()
{
    auto state = sdl::Events::KeyboardState();
    if (state[SDL_SCANCODE_F12])
    {
        RectCollider::hasDebugDraw = !RectCollider::hasDebugDraw;
        Position::hasDebugDraw = !Position::hasDebugDraw;
        TileGrid::hasDebugDraw = !TileGrid::hasDebugDraw;
        CameraData::isFollowing = !CameraData::isFollowing;
    }
}
