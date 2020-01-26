#include "tilegridsystem.hpp"

#include "../core.hpp"

void GridRender()
{
    auto gridView = registry.view<TileGrid, Position>();
    auto cameraView = registry.view<Camera>();
    auto &activeCamera = cameraView.get(*cameraView.begin());

    activeCamera.UpdateWindowSize(sdl::Graphics::Window());
    gridView.each([activeCamera](const auto &grid, const auto &position) {
        SDL_FRect world_tile{position.position.x(), position.position.y(), grid.tileSet->TileWidth() * grid.scale.x(),
                             grid.tileSet->TileHeight() * grid.scale.y()};
        auto screenRect = activeCamera.FromWorldToScreenRect(world_tile);
        SDL_Rect screenPosition = {screenRect.x, screenRect.y};

        int j = grid.cell.size() - 1;
        for (const auto &row : grid.cell)
        {
            int i = 0;
            screenRect.y = static_cast<int>(screenPosition.y - screenRect.h * j);
            for (const auto &id : row)
            {
                if (id)
                {

                    screenRect.x = static_cast<int>(screenPosition.x + screenRect.w * i);
                    if (activeCamera.Contains(screenRect))
                    {
                        sdl::Graphics::RenderToLayer(grid.layer, grid.tileSet->Texture(), &(*grid.tileSet)[id - 1],
                                                     &screenRect);
                        if (TileGrid::hasDebugDraw)
                        {
                            sdl::Graphics::SetDrawColor(0, 0, 255, 255);
                            sdl::Graphics::DrawRectToLayer(6, &screenRect);
                            sdl::Graphics::ResetDrawColor();
                        }
                    }

                    //                    }
                }
                i++;
            }
            j--;
        }
    });
}

void GridCreate()
{
    pugi::xml_document map;
    auto result = map.load_file("map_test.tmx");
    if (!result)
    {
        FAST_THROW(result.description());
    }
    {
        auto id = registry.create();
        auto &tilegrid = registry.assign<TileGrid>(id, map, &tileset, 1);
        registry.assign<Position>(id);
        registry.assign<TileGridCollider>(id);
        registry.assign<CollisionLayer>(id, LayersID::FLOOR);
        registry.assign<Active>(id);
        tilegrid.scale = {2, 2};
        tilegrid.layer = 0;
    }
    {
        auto id = registry.create();
        auto &tilegrid = registry.assign<TileGrid>(id, map, &tileset, 2);
        registry.assign<Position>(id);
        registry.assign<TileGridCollider>(id);
        registry.assign<CollisionLayer>(id, LayersID::WALLS);
        registry.assign<Active>(id);
        tilegrid.scale = {2, 2};
        tilegrid.layer = 1;
    }
    {
        auto id = registry.create();
        auto &tilegrid = registry.assign<TileGrid>(id, map, &tileset, 3);
        registry.assign<Position>(id);
        registry.assign<TileGridCollider>(id);
        registry.assign<CollisionLayer>(id, LayersID::WALLS);
        registry.assign<Active>(id);

        tilegrid.scale = {2, 2};
        tilegrid.layer = 2;
    }
}
