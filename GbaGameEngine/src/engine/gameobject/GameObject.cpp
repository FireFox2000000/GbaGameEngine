#include "GameObject.h"
#include "engine/base/Macros.h"
#include "engine/gameobject/transformation/Transform.h"

using namespace Component;

GameObject::GameObject(ECS::EntityComponentManager* ref) : ECS::ManagedEntity(ref)
{
	// Automatically add transformation components
	AddComponent<Transform>();
}
