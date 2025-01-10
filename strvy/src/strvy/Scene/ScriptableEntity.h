#pragma once

#include "Entity.h"

namespace strvy {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& getComponent()
		{
			return m_entity.getComponent<T>();
		}
	protected:
		virtual void onCreate() {}
		virtual void onUpdate(Timestep ts) {};
		virtual void onDestroy() {};
	private:
		Entity m_entity;
		friend class Scene;
	};

}