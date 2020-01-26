#ifndef FRAMERATE_HPP
#define FRAMERATE_HPP

#include "fwd.hpp"
#include "timer.hpp"

namespace ssecs::sdl
{
class FrameRate
{
    friend Game;

public:
    void OnFrameStart()
    {
        m_deltaTime = static_cast<float>(m_timer.Ellapsed() / 1000.f);
        m_timer.Record();

        dt = m_hasFrameLock ? (1.0f / m_targetFrameRate) : m_deltaTime;
    }

private:
    float dt;

    float m_targetFrameRate = 60;
    bool m_hasFrameLock = true;
    float m_deltaTime = 0;

    Timer m_timer;
};
} // namespace ssecs::sdl

#endif // FRAMERATE_HPP
