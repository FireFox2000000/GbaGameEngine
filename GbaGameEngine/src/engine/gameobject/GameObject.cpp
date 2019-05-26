#include "GameObject.h"
#include "engine/base/Macros.h"
#include "engine/gameobject/transformation/Position.h"

using namespace Component;

GameObject::GameObject(ECS::EntityComponentManager* ref) : ECS::ManagedEntity(ref)
{
	// TODO, automatically add transformation components
	AddComponent<Position>();
}
