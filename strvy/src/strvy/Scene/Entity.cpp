#include "svpch.h"
#include "Entity.h"

namespace strvy {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_entityHandle(handle), m_scene(scene)
	{

	}

}