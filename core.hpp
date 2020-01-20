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

#include "entt/entt.hpp"
#include "ssecs/component/components.hpp"
#include "ssecs/ssecs.hpp"
using namespace ssecs;
using namespace component;

inline entt::registry registry{};
inline sdl::TextureCache textureCache;
inline TileSet tileset;
inline TileSet spriteSheet;

#endif // COMPONENTS_HPP
