#pragma once

#include "svpch.h"

#include "strvy/Core/Core.h"
#include "strvy/Events/Event.h"

namespace strvy {

	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "strvy engine",
					uint32_t width = 1280,
					uint32_t height = 720)
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

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		//Window attributes
		virtual void setEventCallback(const eventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}