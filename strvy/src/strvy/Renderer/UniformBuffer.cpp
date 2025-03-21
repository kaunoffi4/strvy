#include "svpch.h";
#include "UniformBuffer.h"

#include "strvy/Renderer/Renderer.h"
#include "platform/OpenGL/OpenGLUniformBuffer.h"


namespace strvy {

	Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLUniformBuffer>(size, binding);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}

}