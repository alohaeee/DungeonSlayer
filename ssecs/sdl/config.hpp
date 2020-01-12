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

#endif // SDL_CONFIG_HPP
