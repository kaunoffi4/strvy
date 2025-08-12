#include "svpch.h"

#include "Mesh.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLMesh.h"

namespace strvy {

	Ref<Mesh> Mesh::create(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ref<Texture2D>> textures)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:		SV_CORE_ASSERT(false, " rendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:		return createRef<OpenGLMesh>(vertices, indices, textures);
		}

		SV_CORE_ASSERT(false, "Unknown rendererAPI!");
		return nullptr;
	}

}