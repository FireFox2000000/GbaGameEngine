#include "EntityComponentManager.h"
#include "engine/base/Macros.h"

ECS::Entity ECS::EntityComponentManager::CreateEntity()
{
	return m_registry.create();
}

void ECS::EntityComponentManager::DestroyEntity(Entity entity)
{
	// Make sure we destroy these in a safe context. By queuing we can safety destroy any entity in the middle of systems
	m_entitesToDestroyQueue.Add(entity);
}

void ECS::EntityComponentManager::InternalFinaliseDestroy()
{
	for (Entity e : m_entitesToDestroyQueue)
	{
		DEBUG_LOGFORMAT("Destroying entity %d", e);
		m_registry.destroy(e);
	}

	m_entitesToDestroyQueue.Clear();
}
