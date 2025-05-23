#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace strvy {

	class RendererAPI
	{
	public: 
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual void init() = 0;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;
		

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void drawInstanced(const std::shared_ptr<VertexArray>& vertexArray, uint32_t instanceCount) = 0;

		virtual void getMaxUBOSize(int& maxUBOSize) = 0;

		inline static API getAPI() { return s_API; }
	private:
		static API s_API;
		
	};
}