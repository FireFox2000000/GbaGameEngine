#include "GameObject.h"
#include "engine/base/Macros.h"
#include "engine/transform/Transform2.h"
#include "engine/engine/engine.h"

GameObject::GameObject() : ECS::ManagedEntity()
{
	// Automatically add transformation components
	AddComponent<Transform2>();
}
