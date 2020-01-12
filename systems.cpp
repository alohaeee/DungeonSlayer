#include "systems.hpp"

#include "components.hpp"

void Initialization()
{
}

void SpriteRender()
{
    auto view = registry.view<Sprite>();

    for (auto &entity : view)
    {
        auto sprite = view.get<Sprite>(entity);
        SDL_RenderCopy(sdl::CoreData::Renderer(), sprite.texture, &sprite.srcRect, &sprite.dstRect);
    }
}
