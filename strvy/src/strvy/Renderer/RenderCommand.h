#pragma once

#include "RendererAPI.h"

namespace strvy {

	class RenderCommand
	{
	public:
		inline static void setClearColor(const glm::vec4& color)
		{
			s_rendererAPI->setClearColor(color);
		}

		inline static void clear()
		{
			s_rendererAPI->clear();
		}

		inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_rendererAPI->drawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_rendererAPI;
	};

}