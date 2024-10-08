#include "svpch.h"
#include "VertexArray.h"


#include "Renderer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace strvy {

	Ref<VertexArray> VertexArray::create()
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLVertexArray>();
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}
}