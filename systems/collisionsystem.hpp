#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <SDL_rect.h>
void CollisionDetection();

bool AABB(const SDL_FRect &lhs, const SDL_FRect &rhs);

#endif // COLLISIONSYSTEM_HPP
