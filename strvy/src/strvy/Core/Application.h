#pragma once

#include "Core.h"

#include "Window.h"
#include "strvy/Core/LayerStack.h"
#include "strvy/Events/Event.h"
#include "strvy/Events/ApplicationEvent.h"

#include "strvy/Core/Timestep.h"

#include "strvy/ImGui/ImGuiLayer.h"




namespace strvy {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		inline static Application& get() { return *s_instance; }
		inline Window& getWindow() { return *m_window; }

		void close();
	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		bool m_minimized = false;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
		
	private:
		static Application* s_instance;
	};

	// to be defined in CLIENT
	Application* createApplication();
}

