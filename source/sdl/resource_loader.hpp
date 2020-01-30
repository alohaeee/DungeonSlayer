#ifndef TEXTURE_CACHE_HPP
#define TEXTURE_CACHE_HPP

#include <string>

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>

#include "graphics.hpp"

namespace ssecs::sdl
{

/*! @brief Texture Loader struct. */
struct ResourceLoader
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

    static TTF_Font *Font(std::string_view path, int size)
    {
        auto font = TTF_OpenFont(path.data(), size);
        if (!font)
        {
            SDL_THROW();
        }

        return font;
    }

    static SDL_Texture *Text(TTF_Font *font, std::string_view text, SDL_Color color = {0, 0, 0, SDL_ALPHA_OPAQUE})
    {
        auto surface = TTF_RenderUTF8_Blended(font, text.data(), color);
        if (!surface)
        {
            SDL_THROW();
        }

        auto texture = SDL_CreateTextureFromSurface(sdl::Graphics::Renderer(), surface);
        if (!texture)
        {
            SDL_THROW();
        }
        SDL_FreeSurface(surface);
        return texture;
    }
    static Mix_Music *Music(std::string_view path)
    {
        auto music = Mix_LoadMUS(path.data());
        if (!music)
        {
            SDL_THROW();
        }

        return music;
    }
};

} // namespace ssecs::sdl

#endif // TEXTURE_CACHE_HPP
