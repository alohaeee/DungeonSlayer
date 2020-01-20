#ifndef SDL_FWD_HPP
#define SDL_FWD_HPP

#include "../fwd.hpp"
#include <SDL_render.h>

namespace ssecs::sdl
{

/*! @class Process */
class BasicScene;

/*! @class GameEngine */
class GameEngine;

/*! @class TileSet */
class TileSet;

class Game;

class Instances;

/*! @brief Resources cache for texture. */
using TextureCache = ssecs::resource::cache<SDL_Texture *, SDL_DestroyTexture>;

} // namespace ssecs::sdl

#endif // SDL_FWD_HPP
