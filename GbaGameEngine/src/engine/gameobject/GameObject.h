#pragma once

#include "engine/base/ecs/ManagedEntity.h"

class Engine;

// A managed entity that automatically adds transformation components
class GameObject : public ECS::ManagedEntity
{
public:
	GameObject();
};
