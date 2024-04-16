#pragma once

#include "Core.h"


namespace strvy {

	class STRVY_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// to be defined in CLIENT
	Application* createApplication();
}

