#include "svpch.h"
#include "Framebuffer.h"

#include "strvy/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace strvy {



	Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec)
	{

		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLFramebuffer>(spec);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
		return Ref<Framebuffer>();
	}

}