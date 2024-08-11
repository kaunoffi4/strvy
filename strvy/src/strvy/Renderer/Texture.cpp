#include "svpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLTexture.h"


namespace strvy {

	Ref<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}
}