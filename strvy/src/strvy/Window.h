#pragma once

#include "svpch.h"

#include "strvy/Core.h"
#include "strvy/Events/Event.h"

namespace strvy {

	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "strvy engine",
					unsigned int width = 1280,
					unsigned int height = 720)
			: title(title), width(width), height(height) 
		{
		}

	};



	//Interface representing a desktop system based Window
	class STRVY_API Window
	{
	public:
		using eventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		//Window attributes
		virtual void setEventCallback(const eventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}