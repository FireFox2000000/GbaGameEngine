#pragma once
#include "external/entt/entity/registry.hpp"
#include "Entity.h"
#include "engine/base/core/stl/List.h"

/**
* A wrapper class around entt ecs registry to clean up syntax
*/

namespace ECS
{
	class EntityComponentManager
	{
		entt::registry m_registry;
		List<Entity> m_entitesToDestroyQueue = List<Entity>(64);

	public:
		inline Entity CreateEntity()
		{
			return m_registry.create();
		}

		inline void DestroyEntity(Entity entity)
		{
			// Make sure we destroy these in a safe context. By queuing we can safety destroy any entity in the middle of systems
			m_entitesToDestroyQueue.Add(entity);	
		}

		void InternalFinaliseDestroy();

		template<typename Component, typename... Args>
		inline Component& AddComponent(const Entity entity, Args&& ... args)
		{
			return m_registry.assign_or_replace<Component, Args...>(entity, std::forward<Args>(args)...);
		}

		template<typename Component>
		inline void RemoveComponent(const Entity entity)
		{
			return m_registry.remove<Component>(entity);
		}

		template<typename Component>
		inline const Component* GetComponent(const Entity entity) const
		{
			return m_registry.try_get<Component>(entity);
		}

		template<typename Component>
		inline Component* EditComponent(const Entity entity)
		{
			return m_registry.try_get<Component>(entity);
		}

		template<typename Component>
		inline bool HasComponent(const Entity entity)
		{
			return m_registry.has<Component>(entity);
		}

		template<typename... Component>
		inline entt::basic_view<Entity, Component...> View() {
			return m_registry.view<Component...>();
		}

		template<typename... Component, typename Func>
		inline void InvokeEach(Func func) {
			m_registry.view<Component...>().each(func);
		}

		size_t Alive()
		{
			return m_registry.alive();
		}
	};
}