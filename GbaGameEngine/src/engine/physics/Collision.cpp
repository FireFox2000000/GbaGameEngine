#include "Collision.h"

bool Collision::InvolvedEntity(ECS::Entity e) const
{
	return e == entityA || e == entityB;
}
