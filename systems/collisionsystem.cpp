#include "collisionsystem.hpp"

#include "core.hpp"

void CollisionDetection()
{
    auto viewRect = registry.view<RectCollider, Position>();
    auto viewGrid = registry.view<TileGridCollider, Position, TileGrid>();
    auto cameraView = registry.view<Camera>();
    auto &activeCamera = cameraView.get(*cameraView.begin());
    for (auto rect_entt : viewRect)
    {
        auto &&[rect_pos, rect_collider] = viewRect.get<Position, RectCollider>(rect_entt);
        SDL_FRect world_rect{rect_pos.position.x() + rect_collider.rect.x, rect_pos.position.y() + rect_collider.rect.y,
                             rect_collider.rect.w, rect_collider.rect.h};
        for (auto grid_entt : viewGrid)
        {
            auto &&[grid_pos, tile_grid] = viewGrid.get<Position, TileGrid>(grid_entt);
            if (rect_entt != grid_entt)
            {
                SDL_FRect world_tile{grid_pos.position.x(), grid_pos.position.y(),
                                     tile_grid.tileSet->TileWidth() * tile_grid.scale.x(),
                                     tile_grid.tileSet->TileHeight() * tile_grid.scale.y()};
                int j = tile_grid.cell.size() - 1;
                for (auto &row : tile_grid.cell)
                {
                    int i = 0;
                    for (auto &id : row)
                    {

                        if (id)
                        {
                            world_tile.y = grid_pos.position.y() + j * world_tile.h;
                            world_tile.x = grid_pos.position.x() + i * world_tile.w;
                            if (AABB(world_rect, world_tile))
                            {
                                std::cerr << "col" << std::endl;
                                std::cerr << world_tile.x << "\t" << world_tile.y << std::endl;
                                std::cerr << world_rect.x << "\t" << world_rect.y << std::endl;

                                auto test0 = activeCamera.FromWorldToScreenRect(world_tile);

                                sdl::Graphics::SetDrawColor(0, 255, 0, 255);
                                sdl::Graphics::DrawRectToLayerF(4, &test0);
                                sdl::Graphics::SetDrawColor(255, 255, 0, 255);
                                auto test = activeCamera.FromWorldToScreenRect(world_rect);
                                sdl::Graphics::DrawRectToLayerF(4, &test);
                                sdl::Graphics::ResetDrawColor();
                            }
                        }

                        i++;
                    }
                    j++;
                }
            }
        }
    };
}

bool AABB(const SDL_FRect &lhs, const SDL_FRect &rhs)
{
    return !((lhs.x > rhs.x + rhs.w) || (lhs.x + lhs.w < rhs.x) || (lhs.y > rhs.y + rhs.h) || (lhs.y + lhs.h < rhs.y));
}
