#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cmath>

#include <SDL_rect.h>
#include <SDL_video.h>

#include "vector2d.hpp"

namespace ssecs::component
{
class Camera
{

public:
    Camera() = default;
    Camera(const Vector2D &position, const Vector2D &viewRadius) : position{position}, viewRadius{viewRadius}
    {
    }
    Camera(const float x, const float y, const float w, const float h) : position{x, y}, viewRadius{w, h}
    {
    }
    const Vector2D FromScreenViewToWorld(const Vector2D &point) const
    {
        Vector2D world{(point.x() * (2 * viewRadius.x()) / m_windowWidth + position.x() - viewRadius.x()),
                       -(point.y() * (2 * viewRadius.y()) / m_windowHeight + position.y() + viewRadius.y())};
        return world;
    }
    const Vector2D FromWorldToScreenView(const Vector2D &point) const
    {
        Vector2D screen{
            std::round(((point.x() - (position.x() - viewRadius.x())) * m_windowWidth) / (2 * viewRadius.x())),
            -std::round(((point.y() - (position.y() + viewRadius.y())) * m_windowHeight) / (2 * viewRadius.y()))};
        return screen;
    }
    const SDL_Rect FromWorldToScreenRect(const SDL_FRect &rect) const
    {
        Vector2D left_up = FromWorldToScreenView({rect.x, rect.y + rect.h});
        return {static_cast<int>(std::round(left_up.x())), static_cast<int>(std::round(left_up.y())),
                static_cast<int>(std::round(rect.w * m_windowWidth / (2 * viewRadius.x()))),
                static_cast<int>(std::round(rect.h * m_windowHeight / (2 * viewRadius.y())))};
    }

    void UpdateWindowSize(SDL_Window *window) noexcept
    {
        SDL_GetWindowSize(window, &m_windowWidth, &m_windowHeight);
    }

    bool Contains(const Vector2D &point) const noexcept
    {
        return (point.x() >= (position.x() - viewRadius.x()) && (point.x() <= (position.x() + viewRadius.x()))) &&
               (point.y() >= (position.y() - viewRadius.y()) && (point.y() <= (position.y() + viewRadius.y())));
    }

    bool Contains(const SDL_FRect &rect) const noexcept
    {
        return !((rect.x + rect.w < position.x() - viewRadius.x()) || (rect.x > position.x() + viewRadius.x()) ||
                 (rect.y > position.y() + viewRadius.y()) || (rect.y + rect.h < position.y() - viewRadius.y()));
    }

    bool Contains(const SDL_Rect &rect) const noexcept
    {
        return !((rect.x + rect.w < 0) || (rect.x > m_windowWidth) || (rect.y + rect.h < 0) ||
                 (rect.y > m_windowHeight));
    }

    Vector2D position{};
    Vector2D viewRadius{};

private:
    int m_windowWidth;
    int m_windowHeight;
};

struct CameraData
{
    inline static bool isFollowing = true;
};
} // namespace ssecs::component

#endif // CAMERA_HPP
