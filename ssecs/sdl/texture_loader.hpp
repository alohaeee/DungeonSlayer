#ifndef TEXTURE_CACHE_HPP
#define TEXTURE_CACHE_HPP

#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>

#include "../fwd.hpp"
#include "config.hpp"
#include "core_data.hpp"

namespace ssecs::sdl
{

using TextureCache = ssecs::resources::cache<SDL_Texture, SDL_DestroyTexture>;

/*! @brief Texture Loader struct. */
struct TextureLoader
{
    static SDL_Texture *Sprite(const char *path)
    {
        auto surface = IMG_Load(path);
        if (!surface)
        {
            SDL_THROW();
        }

        auto texture = SDL_CreateTextureFromSurface(CoreData::Renderer(), surface);
        if (!texture)
        {
            SDL_THROW();
        }
        SDL_FreeSurface(surface);

        return texture;
    }
};

} // namespace ssecs::sdl

#endif // TEXTURE_CACHE_HPP
