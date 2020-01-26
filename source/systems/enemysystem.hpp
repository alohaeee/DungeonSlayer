#ifndef ENEMYSYSTEM_HPP
#define ENEMYSYSTEM_HPP
#include "../component/colliders.hpp"
#include "../component/vector2d.hpp"
void EnemyCreate(ssecs::component::Vector2D spawn);
void EnemyCharging(const ssecs::component::CollisionData &lhs, const ssecs::component::CollisionData &rhs);
void UpdateView();
void EnemyWalking(const float dt);
void HealthUpdate();
void Respawn();

#endif // ENEMYSYSTEM_HPP
