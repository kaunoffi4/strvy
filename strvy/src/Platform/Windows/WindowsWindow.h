#pragma once

#include "strvy/Core/Window.h"
#include "strvy/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>




namespace strvy {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();


		void onUpdate();

		inline unsigned int getWidth() const override { return m_data.width; }
		inline unsigned int getHeight() const override { return m_data.height; }

		//Window attributes 
		inline void setEventCallback(const eventCallbackFn& callback) override { m_data.eventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		inline void* getNativeWindow() const { return m_window; }
	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();
	private:
		GLFWwindow* m_window;
		GraphicsContext* m_context;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool VSync;

			eventCallbackFn eventCallback;
		};

		WindowData m_data;

	};
}