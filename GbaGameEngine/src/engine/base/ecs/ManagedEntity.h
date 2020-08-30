#pragma once

#include "entt/entity/entity.hpp"
#include "EntityComponentManager.h"

/**
	* @brief A self-managed Entity. Removes itself from the registry when it falls out of scope.
	* Originally based on actor.hpp from skypjack/entt (https://github.com/skypjack/entt)
	*/

namespace ECS
{
	class ManagedEntity {

	private:
		Entity m_entity;
		static EntityComponentManager* s_managedEntityServiceLocation;

	public:
		/**
			* @brief Constructs an actor by using the given registry.
			* @param ref An entity-component system properly initialized.
			*/
		ManagedEntity()
			: m_entity(s_managedEntityServiceLocation->CreateEntity())
		{}

		/*! @brief Default destructor. */
		virtual ~ManagedEntity() {
			s_managedEntityServiceLocation->DestroyEntity(m_entity);
		}

		/**
			* @brief Move constructor.
			*
			* After actor move construction, instances that have been moved from are
			* placed in a valid but unspecified state. It's highly discouraged to
			* continue using them.
			*
			* @param other The instance to move from.
			*/
		ManagedEntity(ManagedEntity&& other)
			: m_entity(other.m_entity)
		{
			other.m_entity = entt::null;
		}

		/**
			* @brief Move assignment operator.
			*
			* After actor move assignment, instances that have been moved from are
			* placed in a valid but unspecified state. It's highly discouraged to
			* continue using them.
			*
			* @param other The instance to move from.
			* @return This actor.
			*/
		ManagedEntity& operator=(ManagedEntity&& other) {
			if (this != &other) {
				auto tmp{ std::move(other) };
				std::swap(m_entity, tmp.m_entity);
			}

			return *this;
		}

		/**
			* @brief Assigns the given component to an actor.
			*
			* A new instance of the given component is created and initialized with the
			* arguments provided (the component must have a proper constructor or be of
			* aggregate type). Then the component is assigned to the actor.<br/>
			* In case the actor already has a component of the given type, it's
			* replaced with the new one.
			*
			* @tparam Component Type of the component to create.
			* @tparam Args Types of arguments to use to construct the component.
			* @param args Parameters to use to initialize the component.
			* @return A reference to the newly created component.
			*/
		template<typename Component, typename... Args>
		inline Component& AddComponent(Args&& ... args) {
			return s_managedEntityServiceLocation->AddComponent<Component>(m_entity, std::forward<Args>(args)...);
		}

		/**
			* @brief Removes the given component from an actor.
			* @tparam Component Type of the component to remove.
			*/
		template<typename Component>
		inline void RemoveComponent() {
			s_managedEntityServiceLocation->RemoveComponent<Component>(m_entity);
		}

		/**
			* @brief Checks if an actor has the given component.
			* @tparam Component Type of the component for which to perform the check.
			* @return True if the actor has the component, false otherwise.
			*/
		template<typename Component>
		inline bool HasComponent() const {
			return s_managedEntityServiceLocation->HasComponent<Component>(m_entity);
		}

		/**
			* @brief Returns references to the given components for an actor.
			* @tparam Component Types of components to get.
			* @return References to the components owned by the actor.
			*/
		template<typename Component>
		inline const Component* GetComponent() const {
			return s_managedEntityServiceLocation->GetComponent<Component>(m_entity);
		}

		template<typename Component>
		inline Component* EditComponent() {
			return s_managedEntityServiceLocation->EditComponent<Component>(m_entity);
		}

		/**
			* @brief Returns the entity associated with an actor.
			* @return The entity associated with the actor.
			*/
		inline Entity GetEntity() const {
			return m_entity;
		}

		static void ProvideEntityManagerService(EntityComponentManager* registry)
		{
			s_managedEntityServiceLocation = registry;
		}
	};
}
