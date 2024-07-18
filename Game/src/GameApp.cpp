#include "strvy.h"


class ExampleLayer : public strvy::Layer
{
public: 
	ExampleLayer()
		: Layer("Example")
	{
	}

	void onUpdate() override
	{
		if (strvy::Input::isKeyPressed(SV_KEY_TAB))
			SV_TRACE("Tab key is pressed!");

	}

	void onEvent(strvy::Event& event) override
	{
		if (event.getEventType() == strvy::EventType::KeyPressed)
		{
			strvy::KeyPressedEvent& e = (strvy::KeyPressedEvent&)event;
			SV_TRACE("{0}", (char)e.getKeyCode());
		}
	}

};


class Game : public strvy::Application
{
public:
	Game()
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new strvy::ImGuiLayer());
	}

	~Game()
	{

	}
};

strvy::Application* strvy::createApplication()
{
	return new Game();
}