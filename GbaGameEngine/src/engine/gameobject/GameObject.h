#pragma once

#include "engine/base/ecs/ManagedEntity.h"

// A managed entity that automatically adds transformation components
class GameObject : public ECS::ManagedEntity
{
public:
	GameObject(ECS::EntityComponentManager* ref);
};
