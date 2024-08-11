#pragma once

#include "strvy/Core/Core.h"
#include "strvy/Events/Event.h"

#include "strvy/Core/Timestep.h"

namespace strvy {

	class STRVY_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return m_debugName; }
	protected:
		std::string m_debugName;

	};
}