#include "svpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGLTexture.h"


namespace strvy {

	Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return createRef<OpenGLTexture2D>(width, height);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return createRef<OpenGLTexture2D>(path);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}
}