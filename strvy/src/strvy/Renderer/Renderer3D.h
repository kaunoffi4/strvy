#pragma once

#include "EditorCamera.h"

#include "strvy/Scene/Components.h"

namespace strvy {

	struct LightBlock;

	class Renderer3D
	{
	public:
		static void init();
		static void shutdown();

		static void beginScene(const EditorCamera& camera);
		static void beginScene(const EditorCamera& camera, const LightBlock& lightBlock);

		static void drawCall(const glm::mat4& transform, const glm::vec4& color, int entityID);

		static void drawPrimitive(const glm::mat4& transform, const PrimitiveComponent& pc, int entityID);


		static void endScene();
	};

}