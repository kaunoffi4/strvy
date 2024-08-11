#pragma once


#ifdef SV_PLATFORM_WINDOWS

extern strvy::Application* strvy::createApplication();

int main(int argc, char** argv)
{
	strvy::Log::init();
	SV_CORE_WARN("Initialized Log!");
	int a = 5;
	SV_INFO("Hello Var={0}", a);

	auto app = strvy::createApplication();
	app->run();
	delete app;
}

#endif