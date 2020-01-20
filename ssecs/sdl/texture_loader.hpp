#ifndef TEXTURE_CACHE_HPP
#define TEXTURE_CACHE_HPP

#include <string>

#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>

#include "graphics.hpp"

namespace ssecs::sdl
{

/*! @brief Texture Loader struct. */
struct TextureLoader
{
    static SDL_Texture *Sprite(std::string_view path)
    {
        auto texture = IMG_LoadTexture(Graphics::Renderer(), path.data());
        if (!texture)
        {
            SDL_THROW();
        }

        return texture;
    }
};

} // namespace ssecs::sdl

#endif // TEXTURE_CACHE_HPP
