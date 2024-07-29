#include "svpch.h"
#include "RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace strvy {

	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}

