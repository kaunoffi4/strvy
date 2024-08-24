#include "svpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace strvy {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		SV_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::init()
	{
		SV_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SV_CORE_ASSERT(status, "Failed to initialize Glad!");

	#ifdef SV_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		SV_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "strvy requires at least OpenGL version 4.5");
	#endif

	}

	void OpenGLContext::swapBuffers()
	{
		SV_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}
}