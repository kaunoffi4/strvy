#pragma once

#include "Core.h"

#include "Window.h"
#include "strvy/LayerStack.h"
#include "strvy/Events/Event.h"
#include "strvy/Events/ApplicationEvent.h"

#include "strvy/ImGui/ImGuiLayer.h"

#include "strvy/Renderer/Shader.h"
#include "strvy/Renderer/Buffer.h"
#include "strvy/Renderer/VertexArray.h"




namespace strvy {

	class STRVY_API Application
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
	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_running = true;
		LayerStack m_layerStack;
		
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<VertexArray> m_vertexArray;

		std::shared_ptr<Shader> m_blueShader;
		std::shared_ptr<VertexArray> m_squareVA;

		
	private:
		static Application* s_instance;
	};

	// to be defined in CLIENT
	Application* createApplication();
}

