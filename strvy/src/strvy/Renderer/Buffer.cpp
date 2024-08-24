#include "svpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace strvy {

	Ref<VertexBuffer> VertexBuffer::create(uint32_t size)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLVertexBuffer>(size);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}
}