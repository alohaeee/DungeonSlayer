#ifndef CORE_DATA_HPP
#define CORE_DATA_HPP

#include <SDL.h>

#include "fwd.hpp"

namespace ssecs::sdl
{

class CoreData
{
    friend class GameEngine;

private:
    static inline SDL_Window *s_window = nullptr;
    static inline SDL_Renderer *s_renderer = nullptr;
    static inline SDL_Event s_event{};

public:
    static const SDL_Window *Window()
    {
        return s_window;
    };
    static SDL_Renderer *Renderer()
    {
        return s_renderer;
    }
    static const SDL_Event &Event()
    {
        return s_event;
    }
};
} // namespace ssecs::sdl

#endif // CORE_DATA_HPP
