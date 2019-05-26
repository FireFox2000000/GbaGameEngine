#pragma once
#include "engine/base/ecs/entt/entity/registry.hpp"

/**
* A wrapper class around entt ecs registry to clean up syntax
*/

namespace ECS
{
	using Entity = entt::registry::entity_type;

	class EntityComponentManager
	{
		entt::registry m_registry;

	public:
		inline Entity CreateEntity()
		{
			return m_registry.create();
		}

		inline void DestroyEntity(Entity entity)
		{
			return m_registry.destroy(entity);
		}

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
		inline void HasComponent(const Entity entity)
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