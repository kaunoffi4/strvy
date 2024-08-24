#pragma once

#include "strvy/Core/Layer.h"

#include "strvy/Events/KeyEvent.h"
#include "strvy/Events/MouseEvent.h"
#include "strvy/Events/ApplicationEvent.h"

namespace strvy {

	class STRVY_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void onAttach() override;
		void onDetach() override;

		void begin();
		void end();

	private:
		float m_Time = 0.0f;


	};
}