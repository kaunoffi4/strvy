#pragma once

#include "Core.h"
#include "Window.h"

namespace strvy {

	class STRVY_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};

	// to be defined in CLIENT
	Application* createApplication();
}

