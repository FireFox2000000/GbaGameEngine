#include "GameObject.h"
#include "engine/base/Macros.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/engine/engine.h"

using namespace Component;

GameObject::GameObject(ECS::EntityComponentManager* ref) : ECS::ManagedEntity(ref)
{
	// Automatically add transformation components
	AddComponent<Transform>();
}

GameObject::GameObject(Engine * engine) : GameObject(engine->GetEntityRegistry())
{
}
