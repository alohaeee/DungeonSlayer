#ifndef SDL_FWD_HPP
#define SDL_FWD_HPP

#include "../fwd.hpp"
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <SDL_ttf.h>

namespace ssecs::sdl
{

/*! @class Process */
class BasicScene;

/*! @class GameEngine */
class GameEngine;

/*! @class TileSet */
class TileSet;

/*! @class Game */
class Game;

/*! @class Instances */
class Instances;

/*! @brief Resources cache for textures. */
using TextureCache = ssecs::resource::cache<SDL_Texture *, SDL_DestroyTexture>;
/*! @brief Resources cache for fonts. */
using FontCache = ssecs::resource::cache<TTF_Font *, TTF_CloseFont>;
/*! @brief Resources cache for audio. */
using MusicCache = ssecs::resource::cache<Mix_Music *, Mix_FreeMusic>;

} // namespace ssecs::sdl

#endif // SDL_FWD_HPP
