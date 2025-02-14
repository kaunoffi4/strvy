#include "svpch.h"

#include "Application.h"

#include "strvy/Core/Log.h"

#include "strvy/Renderer/Renderer.h"

#include "Input.h"

#include <glfw/glfw3.h>

// TESTING 

namespace strvy {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application(const std::string& name)
	{
		SV_PROFILE_FUNCTION();

		SV_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create(WindowProps(name)));
		m_window->setEventCallback(BIND_EVENT_FN(onEvent));

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		SV_PROFILE_FUNCTION();

		Renderer::shutdown();
	}

	void Application::pushLayer(Layer* layer)
	{
		SV_PROFILE_FUNCTION();

		m_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		SV_PROFILE_FUNCTION();

		m_layerStack.pushOverlay(layer);
		layer->onAttach();
	}
	
	void Application::onEvent(Event& e)
	{
		SV_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

		//SV_CORE_TRACE("{0}", e);

		for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
		{
			if (e.handled())
				break;
			(*--it)->onEvent(e);
		}

	}

	void Application::run()
	{
		SV_PROFILE_FUNCTION();

		while (m_running)
		{
			SV_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime(); // Platform::getTime
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized)
			{
				{
					SV_PROFILE_SCOPE("LayerStack onUpdate");

					for (Layer* layer : m_layerStack)
						layer->onUpdate(timestep);
				}

				m_ImGuiLayer->begin();
				{
					SV_PROFILE_SCOPE("LayerStack onImGuiRender");

					for (Layer* layer : m_layerStack)
						layer->onImGuiRender(timestep);
				}
				m_ImGuiLayer->end();
			}
				

			m_window->onUpdate();
		}
	}

	void Application::close()
	{
		m_running = false;
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;

		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		SV_PROFILE_FUNCTION();

		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			m_minimized = true;
			return false;
		}

		m_minimized = false;
		Renderer::onWindowResize(e.getWidth(), e.getHeight());

		return false;
	}
}