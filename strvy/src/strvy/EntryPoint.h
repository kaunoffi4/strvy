#pragma once


#ifdef SV_PLATFORM_WINDOWS

extern strvy::Application* strvy::createApplication();

int main(int argc, char** argv)
{

	printf("strvy engine");
	auto app = strvy::createApplication();
	app->run();
	delete app;
}

#endif