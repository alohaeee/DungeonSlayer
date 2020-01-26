#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SDL_events.h>

#include "fwd.hpp"

namespace ssecs::sdl
{
class Events
{
    friend Game;

public:
    static const SDL_Event &Event() noexcept
    {
        return m_event;
    }
    static const Uint8 *KeyboardState() noexcept
    {
        return SDL_GetKeyboardState(NULL);
    }

private:
    static inline SDL_Event m_event{};
};
} // namespace ssecs::sdl

#endif // EVENT_HANDLER_HPP
