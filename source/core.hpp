#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <cstddef>
#include <filesystem>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include "component/components.hpp"
#include "config.hpp"
#include "entt/entt.hpp"
#include "resource/cache.hpp"
#include "sdl/sdl.hpp"

using namespace ssecs;
using namespace component;

inline entt::registry registry{};
inline sdl::TextureCache textureCache;
inline sdl::FontCache fontCache;
inline TileSet tileset;
inline TileSet spriteSheet;
inline ScoreTable scoreTable;
inline bool isMenu = false;

using collision_type = void(CollisionData, CollisionData);
using collision_signal = entt::sigh<collision_type>;
using collision_sink = entt::sink<collision_type>;

#endif // COMPONENTS_HPP
