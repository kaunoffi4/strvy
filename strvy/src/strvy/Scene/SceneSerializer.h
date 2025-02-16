#pragma once

#include "Scene.h"

namespace strvy {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		
		void serialize(const std::string& filepath); // serialize in text
		void serializeRuntime(const std::string& filepath); // serialize in binary

		bool deserialize(const std::string& filepath);
		bool deserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_scene;
	};

}