#pragma once

#include "EditorCamera.h"
#include "Model.h"

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

		static void drawPrimitive(const void* instanceData, uint32_t size, uint32_t instanceCount);

		static void drawModel(glm::mat4& transform, Ref<Model> model, int entityID);

		static void endScene();

		struct Statistics
		{
			uint32_t drawCalls = 0;
			//uint32_t quadCount = 0;

			//uint32_t getTotalVertexCount() { return quadCount * 4; }
			//uint32_t getTotalIndexCount() { return quadCount * 6; }
		};
		static Statistics getStats();
	};

}