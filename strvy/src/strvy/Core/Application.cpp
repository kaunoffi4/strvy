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

	Application::Application()
	{
		SV_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::create());
		m_window->setEventCallback(BIND_EVENT_FN(onEvent));

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		m_layerStack.pushOverlay(layer);
		layer->onAttach();
	}
	
	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

		//SV_CORE_TRACE("{0}", e);

		for (auto it = m_layerStack.end(); it != m_layerStack.begin(); )
		{
			(*--it)->onEvent(e);
			if (e.handled())
				break;
		}

	}

	void Application::run()
	{
		while (m_running)
		{
			float time = (float)glfwGetTime(); // Platform::getTime
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized)
			{
				for (Layer* layer : m_layerStack)
					layer->onUpdate(timestep);
			}

			m_ImGuiLayer->begin();

			for (Layer* layer : m_layerStack)
				layer->onImGuiRender();

			m_ImGuiLayer->end();
				

			m_window->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;

		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
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