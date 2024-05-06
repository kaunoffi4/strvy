#include "svpch.h"

#include "Application.h"

#include "strvy/Events/ApplicationEvent.h"
#include "strvy/Log.h"

namespace strvy {

	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::create());
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (m_running)
		{
			m_window->onUpdate();
		}
	}
}