#pragma once

#include "Scene.h"
 

namespace strvy {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			SV_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");

			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent()
		{
			SV_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");

			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		template<typename T>
		bool hasComponent()
		{
			return m_scene->m_registry.has<T>(m_entityHandle);
		}

		template<typename T>
		void removeComponent()
		{
			SV_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");

			m_scene->m_registry.remove<T>(m_entityHandle);
		}

		operator bool() const { return m_entityHandle != entt::null; }

	private:
		entt::entity m_entityHandle{ entt::null }; // actual entity ID
		Scene* m_scene = nullptr; // optional
	};
}