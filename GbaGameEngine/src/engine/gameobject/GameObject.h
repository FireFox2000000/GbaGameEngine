#pragma once

#include "engine/base/ecs/ManagedEntity.h"

class Engine;

class GameObject : public ECS::ManagedEntity
{
public:
	GameObject(ECS::EntityComponentManager* ref);
};
