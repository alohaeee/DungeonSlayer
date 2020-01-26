#ifndef SDL_CONFIG_HPP
#define SDL_CONFIG_HPP

#ifndef SDL_THROW_DISABLE
#include <SDL_error.h>
#include <cstring>
#include <exception>

#ifndef FAST_THROW
#define FAST_THROW(message) throw std::exception(message);
#endif
#ifndef SDL_THROW
#define SDL_THROW() throw std::exception(SDL_GetError());
#endif

#else // SDL_DISABLE_ASSERT
#define SDL_THROW(...) ((void)0)
#endif

#ifndef SDL_RENDER_LAYERS
#define SDL_RENDER_LAYERS 10
#endif

#ifndef SSECS_DISABLE_ASSERT
#include <cassert>
#define SSECS_ASSERT(condition) assert(condition)
#else
#define SSECS_ASSERT(...) ((void)0)
#endif

#endif // SDL_CONFIG_HPP
