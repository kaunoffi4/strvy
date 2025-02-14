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
		void onEvent(Event& e) override;

		void begin();
		void end();

		void blockEvents(bool block) { m_blockEvents = block; }
		void setDarkThemeColors();

	private:
		float m_Time = 0.0f;
		bool m_blockEvents = true;

	};
}