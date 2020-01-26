#ifndef TIMER_HPP
#define TIMER_HPP

#include <SDL.h>

namespace ssecs::sdl
{
/*! @brief Timer builded on SDL_GetTicks() */
class Timer
{
private:
    Uint32 m_previous{};

public:
    /*! @brief Return ticks from init SDL subsystems !*/
    static Uint32 Ticks()
    {
        return SDL_GetTicks();
    }

    /*! @brief  !*/
    Uint32 Started()
    {
        return m_previous;
    }

    /*! @brief Return time since last call ellapsed. !*/
    Uint32 Ellapsed()
    {
        return SDL_GetTicks() - m_previous;
    }

    /*! @brief Start recording !*/
    void Record()
    {
        m_previous = SDL_GetTicks();
    }
};
} // namespace ssecs::sdl

#endif // TIMER_HPP
