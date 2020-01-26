#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "../component/components.hpp"
#include <SDL_rect.h>

void CollisionTileDetection(float dt);
void CollisionDetection();
void SetPlayerHealth(int hp);
void SetPlayerScore(int score);

void NullVelocity(const ssecs::component::CollisionData &lhs, const ssecs::component::CollisionData &rhs);
void OnHit(const ssecs::component::CollisionData &lhs, const ssecs::component::CollisionData &rhs);
std::pair<ssecs::component::Vector2D, ssecs::component::Vector2D> AABBW(const SDL_FRect &lhs, const SDL_FRect &rhs);
bool AABB(const SDL_FRect &lhs, const SDL_FRect &rhs);

#endif // COLLISIONSYSTEM_HPP
