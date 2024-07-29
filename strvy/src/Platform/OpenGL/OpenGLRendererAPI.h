#pragma once

#include "strvy/Renderer/RendererAPI.h"

namespace strvy {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;


		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}