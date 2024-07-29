#pragma once

#include "strvy/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace strvy {

	class OpenGLContext : public GraphicsContext
	{
	public:

		OpenGLContext(GLFWwindow* windowHandle);

		void init() override;
		void swapBuffers() override;

	private:
		GLFWwindow* m_windowHandle;
	};
}