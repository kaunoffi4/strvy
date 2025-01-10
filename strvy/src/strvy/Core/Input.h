#pragma once


#include "strvy/Core/Core.h"


namespace strvy {

	class STRVY_API Input 
	{
	public:
		static bool isKeyPressed(int keycode);

		static bool isMouseButtonPressed(int button);
		static std::pair<float, float> getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}