#include "strvy.h"


class Game : public strvy::Application
{
public:
	Game()
	{

	}

	~Game()
	{

	}
};

strvy::Application* strvy::createApplication()
{
	return new Game();
}